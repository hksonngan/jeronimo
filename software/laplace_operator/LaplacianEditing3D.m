%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sorkine, O.; Cohen-Or, D.; Lipman, Y.; Alexa, M.; Rossl, C. & Seidel, H.-P. Laplacian surface editing Proceedings of the 2004 EurographicsACM SIGGRAPH symposium on Geometry processing, ACM, 2004, 175-184
%
% Lipman, Y.; Sorkine, O.; Levin, D. & Cohen-Or, D. Linear rotation-invariant coordinates for meshes ACM Trans. Graph., ACM, 2005, 24, 479-487
%
% Perez, Alvaro. OBJECT DEFORMATION USING LAPLACIAN COORDINATES. Master Thesis. 2010.
% 
% http://igl.ethz.ch/projects/Laplacian-mesh-processing/Laplacian-mesh-editing/laplacian-editing-2D-MATLAB.zip
%
% Matlab Mesh Toolkit http://www.dgp.toronto.edu/~rms/software/matlabmesh/
%
% Toolbox Graph https://www.ceremade.dauphine.fr/~peyre/matlab/graph/content.html
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Author: Alexander Pinzon Fernandez apinzonf@gmail.com 2013
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function Vf = LaplacianEditing3D(xyz,F, static_anchors, handle_anchors, anchors_pos);
    n = length(xyz);
    name = 'gsoc';
    options.name = name; % useful for displaying
    options.symmetrize = 1;
    options.normalize = 0;
    L = compute_mesh_laplacian(xyz,F,'conformal',options); 
    [normal,normalf] = compute_normal(xyz,F);
    
    delta = L*xyz;
    %xyz = xyz';
    L_prime = [   L       zeros(n)   zeros(n)% the x-part
                zeros(n)    L        zeros(n)% the y-part
                zeros(n)  zeros(n)     L];   % the z-part
    L_prime = sparse(L_prime);
    Rings = compute_vertex_ring(F);
    %disp(n);
    nbr_i = zeros(n,1);
    for i=1:n
        ring = [Rings{i}];
        ring = [ring i];
        %disp(ring);
        V = xyz(ring, :)';
        V = [V
            ones(1,length(ring))];
    
        lr = length(ring);
        C = zeros(lr * 3, 7);
        for r=1:lr
            C(r,:)        = [V(1,r)      0       V(3, r)   -V(2,r)   1  0  0];
            C(lr + r,:)   = [V(2,r)   -V(3,r)       0       V(1,r)   0  1  0];
            C(2*lr + r,:) = [V(3,r)    V(2,r)   -V(1,r)       0      0  0  1];
        end

        Cinv  = pinv(C);
        %Cinv = pinv(C'*C) * C';
        s = Cinv(1,:);
        h1 = Cinv(2,:);
        h2 = Cinv(3,:);
        h3 = Cinv(4,:);
        tx = Cinv(5,:);
        ty = Cinv(6,:);
        tz = Cinv(7,:);

        delta_i = delta(i,:)';
        dix = delta_i(1);
        diy = delta_i(2);
        diz = delta_i(3);

        Tdelta =   [dix*s  - diy*h3 + diz*h2
                    dix*h3 + diy*s  - diz*h1
                   -dix*h2 + diy*h1 + diz*s];

        L_prime(i,    [ring (ring + n) (ring + 2*n)]) = L_prime(i,    [ring (ring + n) (ring + 2*n)]) +  (-1)*Tdelta(1,:);
        L_prime(i+n,  [ring (ring + n) (ring + 2*n)]) = L_prime(i+n,  [ring (ring + n) (ring + 2*n)]) +  (-1)*Tdelta(2,:);
        L_prime(i+2*n,[ring (ring + n) (ring + 2*n)]) = L_prime(i+2*n,[ring (ring + n) (ring + 2*n)]) +  (-1)*Tdelta(3,:);
%         %%%%%
        nbrdots = zeros(lr,1);
        ni = normal(:, i)';
        qi = xyz(i,:)';
        
        for r=1:(lr -1)
            qj = V(1:3, r);
            vj = qj - qi;
            vj = vj/max(norm(vj), 0.000001);
            nbrdots(r) = abs(dot(vj,ni));
        end
        [mindot,nbri] = min(nbrdots);
        nbr_i(i) = ring(nbri);
    end
    
    A_prime = L_prime;
    rhs = zeros(3*n,1);
    anch_pos = [];

    anchors = static_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime = [A_prime
             w*((1:(3*n))==anchors(j))
             w*((1:(3*n))==(anchors(j)+n))
             w*((1:(3*n))==(anchors(j)+2*n))];
      rhs = [rhs
         w*xyz(anchors(j),1)
         w*xyz(anchors(j),2)
         w*xyz(anchors(j),3)];
    end
    %anchors_pos
    anchors = handle_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime = [A_prime
             w*((1:(3*n))==anchors(j))
             w*((1:(3*n))==(anchors(j)+n))
             w*((1:(3*n))==(anchors(j)+2*n))];
      rhs = [rhs
         w*anchors_pos(j,1)
         w*anchors_pos(j,2)
         w*anchors_pos(j,3)];
    end
    %fprintf(1, 'A_prime\n');
    %disp(full(A_prime));
    A_prime = sparse(A_prime);
    rhs = sparse(rhs);
    %xyz_col = A_prime\rhs;
    %[Lu_0,U_0,P_0] = lu(A_prime);
    %xyz_col = U_0\(Lu_0\rhs);
    %xyz_col = lsqr(U_0,lsqr(Lu_0,rhs));
    %xyz_col = lsqr( A_prime,rhs);
    % [Cqr,Rqr] = qr(A_prime, rhs);
    %xyz_col = Rqr\Cqr;
clc
%RRR = chol(A_prime'*A_prime);
    xyz_col = (A_prime'*A_prime)\(A_prime'*rhs);
    %xyz_col = RRR\(RRR'\(A_prime'*rhs));
    xyz_prime_nonh = [xyz_col(1:n) xyz_col((n+1):(2*n)) xyz_col((2*n+1):(3*n))];
    %Vf = xyz_prime_nonh;
    
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    [normal_deform,normalf_deform] = compute_normal(xyz_prime_nonh,F);
    new_delta = zeros(n, 3);
    for i=1:n
        j = nbr_i(i);
        pi = xyz(i, :);
        ni = normal(:, i)';
        pj = xyz(j, :);
        uij = pj -pi;
        uij = uij - (dot(uij,ni))*ni;
        uij = uij/max(norm(uij), 0.00001);
        e2 = cross(ni, uij);
        deltai = delta(i,:);
        alpha = dot(ni,deltai);
        beta = dot(uij,deltai);
        gamma = dot(e2,deltai);
        
        pi = xyz_prime_nonh(i,:);
        ni = normal_deform(:, i)';
        pj = xyz_prime_nonh(j,:);
        uij = pj -pi;
        uij = uij - (dot(uij,ni))*ni;
        uij = uij/max(norm(uij), 0.000001);
        e2 = cross(ni, uij);  
        
        new_d = alpha*ni + beta*uij + gamma*e2;
        new_delta(i,:) = new_d;
    end
    A_prime2 = L;
    rhs2 = new_delta;
    anch_pos = [];

    anchors = static_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime2 = [A_prime2
             w*((1:n)==anchors(j))];
      rhs2 = [rhs2
         w*xyz(anchors(j),:)];
    end
    %anchors_pos
    anchors = handle_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime2 = [A_prime2
             w*((1:n)==anchors(j))];
      rhs2 = [rhs2
         w*anchors_pos(j,:)];
    end
    
    xyz_end = [A_prime2\rhs2(:, 1), A_prime2\rhs2(:, 2), A_prime2\rhs2(:, 3)];
    
    Vf = xyz_end ;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    [normal_deform,normalf_deform] = compute_normal(xyz_end,F);
    new_delta = zeros(n, 3);
    for i=1:n
        j = nbr_i(i);
        pi = xyz(i, :);
        ni = normal(:, i)';
        pj = xyz(j, :);
        uij = pj -pi;
        uij = uij - (dot(uij,ni))*ni;
        uij = uij/max(norm(uij), 0.00001);
        e2 = cross(ni, uij);
        deltai = delta(i,:);
        alpha = dot(ni,deltai);
        beta = dot(uij,deltai);
        gamma = dot(e2,deltai);
        
        pi = xyz_end(i,:);
        ni = normal_deform(:, i)';
        pj = xyz_end(j,:);
        uij = pj -pi;
        uij = uij - (dot(uij,ni))*ni;
        uij = uij/max(norm(uij), 0.000001);
        e2 = cross(ni, uij);  
        
        new_d = alpha*ni + beta*uij + gamma*e2;
        new_delta(i,:) = new_d;
    end
    A_prime2 = L;
    rhs2 = new_delta;
    anch_pos = [];

    anchors = static_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime2 = [A_prime2
             w*((1:n)==anchors(j))];
      rhs2 = [rhs2
         w*xyz(anchors(j),:)];
    end
    %anchors_pos
    anchors = handle_anchors;
    w=1;
    for j=1:length(anchors)
      A_prime2 = [A_prime2
             w*((1:n)==anchors(j))];
      rhs2 = [rhs2
         w*anchors_pos(j,:)];
    end
    
    xyz_end = [A_prime2\rhs2(:, 1), A_prime2\rhs2(:, 2), A_prime2\rhs2(:, 3)];
    %Vf = xyz_end ;
    

    
end