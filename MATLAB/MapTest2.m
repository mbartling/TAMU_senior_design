%% Test the Mapping Algorithm
% Michael Bartling
% Myles Foreman
% Jenny Chen
%clear all
close all
example_maps


%% Initialize Variables
% And declare

minRSSI                 = -40;  % Because I said so
myParams.alpha          = 20;    % A higher alpha seems to work better
myParams.geometricR     = 0.8;  % must be greater than 0.5
myParams.numRuns        = 10;    % 3 is the minimum useful number
myParams.pathNoiseMax   = 15;   % minimum noise = 1, max = some giant 
myParams.visualizeMode  = 0;


%% Put the maps in a struct for cleanliness
mapMe.g1 = Ant_g1;
mapMe.g2 = Ant_g2;
mapMe.g3 = Ant_g3;
mapMe.g4 = Ant_b ;
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;

TreasureMap1  = YonderWay( ui_start, ui_end, mapMe.g1, minRSSI,myParams );
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;
TreasureMap2  = YonderWay( ui_start, ui_end, mapMe.g2, minRSSI,myParams  );
ui_start.x   = 1;   %arbitrary start point
ui_start.y   = 1;
ui_end.x     = 300; %arbitrary end point
ui_end.y     = 400;
TreasureMap3  = YonderWay( ui_start, ui_end, mapMe.g3, minRSSI,myParams  );

TreasureMap4  = YonderWay( ui_start, ui_end, mapMe.g4, minRSSI,myParams  );
%TreasureMap.fogOfWar = TreasureMap1.fogOfWar + TreasureMap2.fogOfWar + TreasureMap3.fogOfWar + TreasureMap4.fogOfWar;
figure
hold on
mesh(TreasureMap1.fogOfWar);
mesh(TreasureMap2.fogOfWar);
mesh(TreasureMap3.fogOfWar);
mesh(TreasureMap4.fogOfWar);
contour(Ant_g1,'y','ShowText','on');
contour(Ant_g2,'g','ShowText','on');
contour(Ant_g3,'c','ShowText','on');
contour(Ant_b,'r','ShowText','on');
hold off

figure
hold on
mesh(TreasureMap1.fogOfWar);
title('Ant good 1: map');
contour(Ant_g1,'y','ShowText','on');
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

close_g1 = getHowClose(TreasureMap1.fogOfWar, Ant_g1);
close_g2 = getHowClose(TreasureMap2.fogOfWar, Ant_g2);
close_g3 = getHowClose(TreasureMap3.fogOfWar, Ant_g3);
close_b1 = getHowClose(TreasureMap4.fogOfWar, Ant_b);

TrialCloseness = [close_g1, close_g2, close_g3, close_b1]

