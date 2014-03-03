close all;
example_maps

figure;
contour(Ant_g1,'b','ShowText','on');
hold on
contour(Ant_g2,'g','ShowText','on');
contour(Ant_g3,'c','ShowText','on');
contour(Ant_b,'r','ShowText','on');
xlabel('0.1 meters per division');
ylabel('0.1 meters per division');
title('Original Field');
hold off

%% Parameters
xStart      = 50;
xEnd        = 250;
yStart      = 100;
yEnd        = 400;
xres        = 50;
yres        = 50;

rfMap_b = Static_Map2(Ant_b, xres, yres, xStart, xEnd, yStart, yEnd);
rfMap_g1 = Static_Map2(Ant_g1, xres, yres, xStart, xEnd, yStart, yEnd);
rfMap_g2 = Static_Map2(Ant_g2, xres, yres, xStart, xEnd, yStart, yEnd);
rfMap_g3 = Static_Map2(Ant_g3, xres, yres, xStart, xEnd, yStart, yEnd);

figure; imshow(min(min(rfMap_b.gaussPy_small)) -rfMap_b.gaussPy_small + rfMap_g1.gaussPy_small + rfMap_g2.gaussPy_small + rfMap_g3.gaussPy_small,[]);
%axis image; axis on;

figure;
contour(rfMap_g1.gaussPy_small,'b','ShowText','on');
hold on
contour(rfMap_g2.gaussPy_small,'g','ShowText','on');
contour(rfMap_g3.gaussPy_small,'c','ShowText','on');
contour(rfMap_b.gaussPy_small,'r','ShowText','on');
xlabel('0.1 meters per division');
ylabel('0.1 meters per division');
title('Reconstructed Field');
hold off

dlmwrite('map_b.csv', rfMap_b.gaussPy);
dlmwrite('map_g1.csv', rfMap_g1.gaussPy);
dlmwrite('map_g2.csv', rfMap_g2.gaussPy);
dlmwrite('map_g3.csv', rfMap_g3.gaussPy);