%% Test the Mapping Algorithm
% Michael Bartling
% Myles Foreman
% Jenny Chen
%clear all
close all
example_maps


%% Initialize Variables
% And declare

minRSSI = -40; % Because I said so

%% Put the maps in a struct for cleanliness
mapMe.g1 = Ant_g1;
mapMe.g2 = Ant_g2;
mapMe.g3 = Ant_g3;
mapMe.g4 = Ant_b ;
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;

TreasureMap1  = MapHack( ui_start, ui_end, mapMe.g1, minRSSI );
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;
TreasureMap2  = MapHack( ui_start, ui_end, mapMe.g2, minRSSI );
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;
TreasureMap3  = MapHack( ui_start, ui_end, mapMe.g3, minRSSI );

TreasureMap4  = MapHack( ui_start, ui_end, mapMe.g4, minRSSI );
%TreasureMap.fogOfWar = TreasureMap1.fogOfWar + TreasureMap2.fogOfWar + TreasureMap3.fogOfWar + TreasureMap4.fogOfWar;
figure
hold on
mesh(TreasureMap1.fogOfWar);
mesh(TreasureMap2.fogOfWar);
mesh(TreasureMap3.fogOfWar);
mesh(TreasureMap4.fogOfWar);
hold off

figure
hold on
mesh(TreasureMap1.fogOfWar);
title('Ant good 1: map');
contour(Ant_g1,'b','ShowText','on');
hold off


figure
hold on
mesh(TreasureMap2.fogOfWar);
title('Ant good 2: map');
contour(Ant_g2,'g','ShowText','on');
hold off

figure
hold on
mesh(TreasureMap3.fogOfWar);
title('Ant good 3: map');
contour(Ant_g3,'c','ShowText','on');
hold off

figure
hold on
mesh(TreasureMap4.fogOfWar);
title('Ant bad: map');
contour(Ant_b,'r','ShowText','on');
hold off
N           = 55;
mu_x        = 0;
mu_y        = 0;
sigma_x     = 1;
sigma_y     = 1;
scale       = 1;
withRound   = 0;
normalize   = 1;
%mask = my2DGaussian( N, mu_x, mu_y, sigma_x, sigma_y ,scale ,withRound, normalize);
%TreasureMap.fogOfWar = myPxPApplyMask(  TreasureMap.fogOfWar, mask);
%mesh(TreasureMap.fogOfWar);
%figure
%scatter(TreasureMap.path1.x, TreasureMap.path1.y)
%stem3(TreasureMap.path1.x,TreasureMap.path1.y,TreasureMap.path1.samples);figure(gcf);