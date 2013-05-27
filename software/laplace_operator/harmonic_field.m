function  [Scalar_Field_U, Vector_Field_G1, Vector_Field_G2, Face_Field_G1, Face_Field_G2]  = harmonic_field( V, F, constraints);
    name = 'harmonic';
    options.name = name;
    options.symmetrize = 1;
    options.normalize = 0;
    [normal,normalf] = compute_normal(V,F);
    L = compute_mesh_laplacian(V,F,'conformal',options);
    C = constraints;

    cfi = find(C);
    for i=1:length(cfi)
         ind = cfi(i);
          %L(:, ind) = 0;
          L(ind, :) = 0;
          L(ind, ind) = 1;
    end

    [Lu_0,U_0,P_0] = lu(L);
    U = U_0\(Lu_0\C');
    Scalar_Field_U = U;

    AG = zeros(3, length(F)*3);
    for i=1:length(F)
        xi = V(:,F(1,i));
        xj = V(:,F(2,i));
        xk = V(:,F(3,i));
        n = normalf(:,i);
        AG(:,(i-1)*3 + 1) = xj - xi;
        AG(:,(i-1)*3 + 2) = xk - xj;
        AG(:,(i-1)*3 + 3) = n;
    end

    UG = zeros(1, length(F)*3);
    for i=1:length(F)
        ui = U(F(1,i));
        uj = U(F(2,i));
        uk = U(F(3,i));
        UG((i-1)*3 + 1) = uj - ui;
        UG((i-1)*3 + 2) = uk - uj;
    end

    MG = zeros(3, length(F));
    RG = zeros(3, length(F));
    v3 = zeros(3, length(V));
    v4 = zeros(3, length(V));

    for i=1:length(F)
        aga = zeros(3,3);
        aga(:,1) = AG(:,(i-1)*3 + 1);
        aga(:,2) = AG(:,(i-1)*3 + 2);
        aga(:,3) = AG(:,(i-1)*3 + 3);
        bg = zeros(1,3);
        bg(1) = UG((i-1)*3 + 1);
        bg(2) = UG((i-1)*3 + 2);
        bg(3) = UG((i-1)*3 + 3);
        msolx = pinv(aga)*bg';
        MG(1, i) = msolx(1);
        MG(2, i) = msolx(2);
        MG(3, i) = msolx(3);
        RG(:,i) = cross(normalf(:, i), MG(:,i));
        vmg = MG(:, i);
        v3(:, F(1,i)) = v3(:, F(1,i)) + vmg;
        v3(:, F(2,i)) = v3(:, F(2,i)) + vmg;
        v3(:, F(3,i)) = v3(:, F(3,i)) + vmg;
        vmg = RG(:, i);
        v4(:, F(1,i)) = v4(:, F(1,i)) + vmg;
        v4(:, F(2,i)) = v4(:, F(2,i)) + vmg;
        v4(:, F(3,i)) = v4(:, F(3,i)) + vmg;
    end
    Vector_Field_G1 = MG;
    Vector_Field_G2 = RG;
    Face_Field_G1 = v3;
    Face_Field_G2 = v4;
end

