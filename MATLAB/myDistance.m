function [ D ] = myDistance( byteMe )
%myDistance Calculate simple distance function
%   byteMe = input image

[P, Q] = size(byteMe);
D = zeros(size(byteMe));
for u = 1:P;
    for v = 1:Q;
        D(u,v) = sqrt((u-P/2)^2 + (v-Q/2)^2);
    end
end


end

