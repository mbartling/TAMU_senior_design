function [ D ] = myDistance( byteMe, centerX, centerY )
%myDistance Calculate simple distance function
%   byteMe = input image

[P, Q] = size(byteMe);
D = zeros(size(byteMe));
for u = 1:P;
    for v = 1:Q;
        D(u,v) = sqrt((u-centerX)^2 + (v-centerY)^2);
    end
end


end

