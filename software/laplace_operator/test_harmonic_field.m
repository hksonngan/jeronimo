function test_harmonic_field();
    [FileName,PathName,FilterIndex] = uigetfile('*.off','Select the off-file');
    [V, F] = read_off(strcat(PathName, FileName));
    C = get_mesh_constraint(FileName, length(V));
    [uf, vf1, vf2, ff1, ff2] = harmonic_field(V,F,C);
    clf;
    %options.normal = v3*10000;
    %options.normal2 = v4*10000;
    %options.face_vertex_color = U;
    %options.face_vertex_color = v3';
    options.face_vertex_color = perform_saturation(abs(uf)+abs(uf),1.2);
    plot_mesh(V, F, options);shading interp; colormap prism(32);
end