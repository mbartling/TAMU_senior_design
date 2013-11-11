function [ thisClose ] = getHowClose( mapA, referenceMap)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
[sizeX,sizeY] = size(mapA);
mapA = getMaxLoc(mapA);
referenceMap = getMaxLoc(referenceMap);
distance = sqrt(0.5*((mapA.x - referenceMap.x)^2+(mapA.y-referenceMap.y)^2));

thisClose = 1-distance/sqrt(sizeX*sizeY);
end

