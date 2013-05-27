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
    
    C = getConstraints(FileName, nv);
    [uf, vf1, vf2, ff1, ff2] = harmonic_field(V',F, C);

    WL = 10* diag(abs(uf));
    %WL = 10;
    SL = 2;
    WH = speye(nv);

    A = [WL*L; WH];
    B = [zeros(nv, 3);  WH*V];
    At_0 = compute_area_ring(F,V');
    lock = zeros(1, nv);
    Vt_i = V;
    
    fprintf(1, '\n');
    for iteration=1:iter
        fprintf(1, 'Iteration: %d\n', iteration);
        
        %[Lu_0,U_0,P_0] = lu(A);
        %Vt_i = U_0\(Lu_0\B);
        
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

function C = getConstraints(name, n);
    C = zeros(1,n);
    if strcmp(name, 'elephant.off')
        C(767) = -1;
        C(960) = -1;
        C(2720) = -1;
        C(1716) = -1;
        C(2266) = -1; 
        C(1309) = -1;
        C(2419) = -1;
        C(1195) = -1;
        C(1692) = -1;
        C(1503) = -1;
        C(1564) = -1;
    elseif strcmp(name, 'cactus.off')
        C(160) = -1;
        C(176 ) = -1;
        C(956 ) = -1;
        C(183 ) = -1;
        C(128 ) = -1;
    elseif strcmp(name, 'MunConCambios.off')
        C(1211) = -1;
        C(136) = -1;
        C(964) = -1;
        C(1143) = -1;
    elseif strcmp(name, 'Motionblender31.off')
        C(538 ) = -1;
        C(240 ) = -1;
        C(287 ) = -1;
        C(289 ) = -1;
        C(251 ) = -1;
        C(487 ) = -1;
        C(487 ) = -1;
        C(465 ) = -1;
        C(25 ) = -1;
    elseif strcmp(name, 'mesh_0120.off')
        C(9645) = -5;
        C(4356) = -1;
        C(4317) = -1;
        C(102) = -1;
        C(29) = -1;
    elseif strcmp(name, 'dino.off')
        C(6548) = -2;
        C(4965) = -1;
        C(20197) = -1;
        C(11562) = -1;
        C(23018) = -1;
        C(15601) = -1;
    elseif strcmp(name, 'FE_final.off')
        C(96460)=-1;
        C(169340)=-1;
        C(168357)=-1;
    elseif strcmp(name, 'cubo.off')
        C(5)=-10;
        C(3)=-1;
    end
end