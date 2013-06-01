function  [VV, F]  = skeletonizer_user( iter );
    [FileName,PathName,FilterIndex] = uigetfile('*.off','Select the off-file');

    [V, F] = read_off(strcat(PathName, FileName));
    V = V';
    options.name = 'skeleton'; % useful for displaying
    options.symmetrize = 1;
    options.normalize = 0;
    L =  compute_mesh_laplacian(V,F,'conformal',options);
    
    nv = length(V);
    nf = length(F);
    dv = (L*V)';
    
    C = get_mesh_constraint(FileName, nv);
    [uf, vf1, vf2, ff1, ff2] = harmonic_field(V',F, C);

    %WL = 10* diag(abs(uf));
    WL = 10;
    SL = 2;
    WH = speye(nv);

    A = [WL*L; WH];
    B = [zeros(nv, 3);  WH*V];
    At_0 = compute_area_ring(F,V');
    lock = zeros(1, nv);
    for i=1:length(C)
        if C(i) ~= 0
            lock(1,i) = 1;
            A(i,:) = 0;
            A(i,i) = 1;
            B(i,:) = V(i,:);
        end
    end
    Vt_i = V;
    
    fprintf(1, '\n');
    for iteration=1:iter
        fprintf(1, 'Iteration: %d\n', iteration);
        
        %[Lu_0,U_0,P_0] = lu(A);
        %Vt_i = U_0\(Lu_0\B);
        for i=1:length(C)
            if C(i) ~= 0
                lock(1,i) = 1;
                A(i,:) = 0;
                A(i,i) = 1;
                B(i,:) = V(i,:);
            end
        end
        Vtemp_i = (A'*A)\(A'*B); 
        for j=1:nf
            if compute_area_face( F,Vtemp_i', j) < 0.00000000001
                lock(1, F(1, j)) = 1;
                lock(1, F(2, j)) = 1;
                lock(1, F(3, j)) = 1;
            end
        end
        for j=1:nv
            if lock(1,j) == 0
                Vt_i(j,:) = Vtemp_i(j,:);
            end
        end
        WL = SL * WL;
        At_i = compute_area_ring(F,Vt_i');
        Lt_i = compute_mesh_laplacian(Vt_i,F,'conformal',options);
        for j=1:nv
            area_j = At_i(j);
            if area_j < 0.000001
                lock(1,j) = 1;
            else
                WH(j,j) = WH(j,j) * sqrt(At_0(j)/area_j);
            end
        end
        B = [zeros(nv, 3); WH*Vt_i];
        A = [WL*Lt_i; WH];
        for j=1:nv
            if lock(1,j) == 1
                A(j, :) = 0;
                A(j, j) = 1;
                B(j, :) = Vt_i( j, :);
            end
        end
        vol = compute_volume(F,Vt_i');
        if vol < 0.00001
            fprintf(1, 'Volume: %f\n', vol);
            break;
        end
        fprintf(1, 'Volume: %f\n', vol);
    end
    VV  = Vt_i ;
end

function AreaR = compute_area_ring(F,V);
    AreaR = zeros(1, length(V));
    for i=1:length(F)
       area = compute_area_face(F, V, i);
       AreaR(F(1,i)) = AreaR(F(1,i)) + area;
       AreaR(F(2,i)) = AreaR(F(2,i)) + area;
       AreaR(F(3,i)) = AreaR(F(3,i)) + area;
    end
end

function area = compute_area_face(F,V, i);
   v1 = V(:, F(1, i));
   v2 = V(:, F(2, i));
   v3 = V(:, F(3, i));
   area = compute_area_vector(v1, v2, v3);
end

function area = compute_area_vector(v1,v2,v3);
    a = v2 - v1;
    b = v3 - v1; 
    c = cross(a,b);
    area = norm(c) /2;
end

function vol = compute_volume(F, V);
	vol = 0;
    numFaces = length(F);
	%float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	for i = 1:numFaces
		v1 = V(:, F(1, i));
        v2 = V(:, F(2, i));
        v3 = V(:, F(3, i));

		x1 = v1(1);
		y1 = v1(2);
		z1 = v1(3);

		x2 = v2(1);
		y2 = v2(2);
		z2 = v2(3);

		x3 = v3(1);
		y3 = v3(2);
		z3 = v3(3);

		vol = vol + (1.0 / 6.0) * (x2 * y3 * z1 + x3 * y1 * z2 - x1 * y3 * z2 - x2 * y1 * z3 + x1 * y2 * z3 - x3 * y2 * z1);
    end
    vol = abs(vol);
end