function LaplacianEditing3D(xyz,F, Fixed_Points, Edited_Points);
    n = length(V);
    name = 'gsoc';
    options.name = name; % useful for displaying
    options.symmetrize = 1;
    options.normalize = 0;
    L = compute_mesh_laplacian(V,F,'conformal',options); 
    xyz = xyz';
    delta = L*xyz;
    L_prime = [   L       zeros(n)   zeros(n)% the x-part
                zeros(n)    L        zeros(n)
                zeros(n)  zeros(n)     L]; % the y-part
    Rings = compute_vertex_face_ring(F);
    for i=1:n
        ring = [Rings{i}];
        ring = [ring i];
        V = xyz(ring, :)';
        V = [V
            ones(1,length(ring))];
    end
    lr = length(ring);
    C = zeros(lr * 3, 7);
    for r=1:length(ring)
        C(r,:)        = [V(1,r)      0       V(3, r)   -V(2,r)   1  0  0];
        C(lr + r,:)   = [V(2,r)   -V(3,r)       0       V(1,r)   0  1  0];
        C(2*lr + r,:) = [V(3,r)    V(2,r)   -V(1,r)       0      0  0  1];
    end
    
    Cinv  = pinv(C);
    s = Cinv(1,:);
    h = Cinv(2,:);
    
    delta_i = delta(i,:)';
    delta_ix = delta_i(1);
    delta_iy = delta_i(2);
    delta_iz = delta_i(3);
    
      Tdelta = [delta_ix*s      + delta_iy* 
                delta_ix*(-1)*a + delta_iy*s];
end