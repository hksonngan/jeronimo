function test_skeletonizer_user();
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\laplace_operator');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\laplace_operator\toolbox_graph');
addpath('E:\personal\maestria\tesis\Jeronimo\trunk\software\laplace_operator\toolbox_graph\toolbox');
    [V,F] = skeletonizer_user(5);
    options.name = 'skeleton';
    plot_mesh(V, F, options);shading interp; colormap jet(32);
end
