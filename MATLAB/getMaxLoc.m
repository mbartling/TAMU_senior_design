function [ maxLoc ] = getMaxLoc( mapA)
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here
maxLoc.maxVal = max(max(mapA));
[sizeX,sizeY] = size(mapA);

for i = 1:sizeX
   for j = 1:sizeY
      if(mapA(i,j) == maxLoc.maxVal)
         maxLoc.x = i;
         maxLoc.y = j;
      end
   end
end

end

