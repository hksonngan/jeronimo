function  [M, F]  = skeletonizer( iter );

addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test\toolbox_graph');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test\toolbox_graph\toolbox');


[FileName,PathName,FilterIndex] = uigetfile('*.off','Select the off-file');
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

[V, F] = read_off(strcat(PathName, FileName));

options.name = 'skeleton'; % useful for displaying
options.symmetrize = 1;
options.normalize = 0;
L =  compute_mesh_laplacian(V,F,'conformal',options);

nv = length(V);
nf = length(F);

A0 = zeros(1,nf);

for i=1:nf 
   A0(i) = arealf(F, V, i);
end
A_avg_0 = mean(A0);

At_0 = arearing(F,V);

%WL = sqrt(A_avg_0) / 1000;
WL = 10;
SL = 2;
WH = speye(nv);

M_A = [WL*L; WH];
%M_A = [speye(nv, nv) + 0.2*L; WH];
whv = WH*V';
M_B = [zeros(nv, 3);  whv];
%M_B = [V  whv'];

tol = 1.e-6;
maxit = 1000;
[smbx, smby] = size(M_B);
fprintf(1, 'size %d, %d\n', smbx, smby);
[smbx, smby] = size(M_B(1,:)');
fprintf(1, 'size %d, %d', smbx, smby);


M_XYZ = (M_A'*M_A)\(M_A'*M_B); 
%[Lu_0,U_0,P_0] = lu(M_A);
%M_XYZ = U_0\(Lu_0\M_B);
%(A'*A)\(A'*B);
%U = cgs(L, C', tol, maxit);
%U = linsolve(L, C');


%M_XYZ(1,:) = lsqr(M_A, M_B(1,:)', tol, maxit);
%M_XYZ(2,:) = lsqr(M_A, M_B(2,:)', tol, maxit);
%M_XYZ(3,:) = lsqr(M_A, M_B(3,:)', tol, maxit);

lockv = zeros(1,nv);

for i=1:(iter-1)
    fprintf(1, 'Iter: %d\n', i);
    
    WL = SL * WL;
    At = arearing(F,M_XYZ');
    Lt =  compute_mesh_laplacian(M_XYZ,F,'conformal',options);
    for j=1:nv
        marea = At(j);
        if marea < 0.000001
            WH(j,j) = 1000;
        else
            WH(j,j) = WH(j,j) * sqrt(At_0(j)/At(j));
        end
        
    end
    
    whv = WH*M_XYZ;
    M_B = [zeros(nv, 3); whv];
    %M_B = [M_XYZ  whv'];

    
    for k=1:nf 
       area2 = arealf(F, M_XYZ', k);
       if area2 < 0.00000001;
           Lt(F(1, k),:) = 0;
           %Lt(F(1, k), F(1, k)) = 1;
           Lt(F(2, k),:) = 0;
           %Lt(F(2, k), F(2, k)) = 1;
           Lt(F(3, k),:) = 0;
           %Lt(F(3, k), F(3, k)) = 1;
           %M_B(F(1, k),:) = M_XYZ(F(1, k),:);
           %M_B(F(2, k),:) = M_XYZ(F(2, k),:);
           %M_B(F(3, k),:) = M_XYZ(F(3, k),:);
           lockv(1, F(1, k)) = 1;
           lockv(1, F(2, k)) = 1;
           lockv(1, F(3, k)) = 1;
           %fprintf(1, 'lock');
       end
    end
    
    M_A = [WL*Lt; WH];
    %M_A = [speye(nv, nv) + WL*0.2*Lt; WH];
    
    %M_XYZ(1,:) = lsqr(M_A, M_B(1,:)', tol, maxit);
    %M_XYZ(2,:) = lsqr(M_A, M_B(2,:)', tol, maxit);
    %M_XYZ(3,:) = lsqr(M_A, M_B(3,:)', tol, maxit);
    tempvert = (M_A'*M_A)\(M_A'*M_B); 
    %[Lu_0,U_0,P_0] = lu(M_A);
    %tempvert = U_0\(Lu_0\M_B);
%     tempvert(1,:) = lsqr(M_A, M_B(1,:)', tol, maxit);
%     tempvert(2,:) = lsqr(M_A, M_B(2,:)', tol, maxit);
%     tempvert(3,:) = lsqr(M_A, M_B(3,:)', tol, maxit);
    for k=1:nv
       if lockv(1, k) == 0
           M_XYZ(k, :) = tempvert(k, :);
       end
    end
   
end


M = M_XYZ;
end

function AreaR = arearing(F,V);
    AreaR = zeros(1, length(V));
    for i=1:length(F)
       area = arealf(F, V, i);
       AreaR(F(1,i)) = AreaR(F(1,i)) + area;
       AreaR(F(2,i)) = AreaR(F(2,i)) + area;
       AreaR(F(3,i)) = AreaR(F(3,i)) + area;
    end
end

function area = arealf(F,V, i);
   v1 = V(:, F(1, i));
   v2 = V(:, F(2, i));
   v3 = V(:, F(3, i));
   area = areaf(v1, v2, v3);
end

function area = areaf(v1,v2,v3);
    a = v2 - v1;
    b = v3 - v1; 
    c = cross(a,b);
    area = norm(c) /2;
end