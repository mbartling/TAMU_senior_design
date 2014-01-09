function [ Trial] = ProfileYonderway_Simple( TestCase )
% alpha, geometricR, numRuns, pathNoiseMax
%UNTITLED10 Summary of this function goes here
%   Detailed explanation goes here
%% Test the Mapping Algorithm
% Michael Bartling
% Myles Foreman
% Jenny Chen


example_maps_noDisplay


%% Initialize Variables
% And declare

minRSSI                 = -40;           % Because I said so
myParams.alpha          = TestCase.alpha;         % A higher alpha seems to work better
myParams.geometricR     = TestCase.geometricR;    % must be greater than 0.5
myParams.numRuns        = TestCase.numRuns;       % 3 is the minimum useful number
myParams.pathNoiseMax   = TestCase.pathNoiseMax;  % minimum noise = 1, max = some giant 
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

close_g1 = getHowClose(TreasureMap1.fogOfWar, Ant_g1);
close_g2 = getHowClose(TreasureMap2.fogOfWar, Ant_g2);
close_g3 = getHowClose(TreasureMap3.fogOfWar, Ant_g3);
close_b1 = getHowClose(TreasureMap4.fogOfWar, Ant_b);

Trial.TrialCloseness = [close_g1, close_g2, close_g3, close_b1];
Trial.TrialParams =     [myParams.alpha, myParams.geometricR, myParams.numRuns, myParams.pathNoiseMax];


end

