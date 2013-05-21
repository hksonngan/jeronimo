addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test\toolbox_graph');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\Laplacian_test\toolbox_graph\toolbox');
clear;
clc;
clf;
%[V,F] = read_off('E:\media3D\elephant.off');
%[V,F] = read_off('E:\media3D\cactus.off');
%[V,F] = read_off('E:\media3D\MunConCambios.off');
%[V,F] = read_off('E:\media3D\Motionblender31.off');
[V,F] = read_off('E:\media3D\mesh_0120.off');
%[V,F] = read_off('E:\media3D\dino.off');
%[V,F] = read_off('E:\media3D\FE_final.off');

name = 'quadmesh';
options.name = name; % useful for displaying
options.symmetrize = 1;
options.normalize = 0;
[normal,normalf] = compute_normal(V,F);
%L = full(compute_mesh_laplacian(V,F,'distance',options));
L = compute_mesh_laplacian(V,F,'distance',options);
[Umin,Umax,Cmin,Cmax,Cmean,Cgauss,Normal] = compute_curvature(V,F,options);
mymin = min(Cmax);
mymax = max(Cmax);
mymin2 = min(Cmin);
mymax2 = max(Cmin);

C = zeros(1,length(V));

% for i=1:length(V)
%     if(Cmax(i)> (mymin + (mymax-mymin)*0.85))
%         C(i) = 1;
%     end
% end
% for i=1:length(V)
%     if(Cmin(i)< (mymin2 + (mymax2-mymin2)*0.07));
%         C(i) = -1;
%     end
% end

%elephant
% C(767) = -1;
% C(960) = -1;
% C(2720) = -1;
% C(1716) = -1;
% C(2266) = -1; 
% C(1309) = -1;
% C(2419) = -1;
% C(1195) = -1;
% C(1692) = -1;
% C(1503) = -1;
% C(1564) = -1;
%%%%%%%%%%%%%%%%%
%cacttus
% C(1201 ) = -1;
% C(947 ) = -1;
% C(240 ) = -1;
% C(986 ) = -1;
% C(511 ) = -1;

%MunConCambios
% C(1211) = -1;
% C(136) = -1;
% C(964) = -1;
% C(1143) = -1;

%Motion31blender
% C(538 ) = -1;
% C(240 ) = -1;
% C(287 ) = -1;
% C(289 ) = -1;
% C(251 ) = -1;
% C(487 ) = -1;
% C(487 ) = -1;
% C(465 ) = -1;
% C(25 ) = -1;

%mesh120
C(9645) = -5;
C(4356) = -1;
C(4317) = -1;
C(102) = -1;
C(29) = -1;

%dino
% C(6548) = -2;
% C(4965) = -1;
% C(20197) = -1;
% C(11562) = -1;
% C(23018) = -1;
% C(15601) = -1;

%FE_final
% C(96460)=-1;
% C(169340)=-1;
% C(168357)=-1;


cfi = find(C);
for i=1:length(cfi)
     ind = cfi(i);
      %L(:, ind) = 0;
      L(ind, :) = 0;
      L(ind, ind) = 1;
 end


tol = 1.e-6;
maxit = 1000;
[Lu_0,U_0,P_0] = lu(L);
%(A'*A)\(A'*B);
%U = cgs(L, C', tol, maxit);
%U = linsolve(L, C');
U = U_0\(Lu_0\C');

% IV = speye(length(V));
% A2 = IV  + 10* L;
% V2 = zeros(3, length(V));

% V2(1,:) =  cgs(A2, V(1,:)', 0.00000001, 100);
% V2(2,:) =  cgs(A2, V(2,:)', 0.00000001, 100);
% V2(3,:) =  cgs(A2, V(3,:)', 0.00000001, 100);


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
    vmg = RG(:, i);
    v3(:, F(1,i)) = v3(:, F(1,i)) + vmg;
    v3(:, F(2,i)) = v3(:, F(2,i)) + vmg;
    v3(:, F(3,i)) = v3(:, F(3,i)) + vmg;
    vmg = MG(:, i);
    v4(:, F(1,i)) = v4(:, F(1,i)) + vmg;
    v4(:, F(2,i)) = v4(:, F(2,i)) + vmg;
    v4(:, F(3,i)) = v4(:, F(3,i)) + vmg;
end

for i=1:length(V)
    v3(:,i) = v3(:,i) / norm(v3(:,i));
    v4(:,i) = v4(:,i) / norm(v4(:,i));
end



clf;
%options.normal = v3*10000;
%options.normal2 = v4*10000;
%options.face_vertex_color = U;
%options.face_vertex_color = v3';
options.face_vertex_color = perform_saturation(abs(U)+abs(U),1.2);

%options.face_color = 'none';
plot_mesh(V, F, options);shading interp; colormap prism(64);
%options.normal = [];