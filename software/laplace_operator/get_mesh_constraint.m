function C = get_mesh_constraint(name, n);
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
    elseif strcmp(name, 'sphere.off')
        C(336)=-1;
        C(369)=1;
        C(351)=-1;
    end
end