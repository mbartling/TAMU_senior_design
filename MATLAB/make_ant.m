function [ theAnt ] = make_ant( stdev, mean, size, scale )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
yo = size/2;
xo = size/2;
theAnt = zeros(size);

for x = 1:size
   for y = 1:size
       theAnt(x,y) = scale*exp(-((x-xo)^2/(2*stdev^2) + (y-yo)^2/(2*stdev^2)));
   end
end

end

