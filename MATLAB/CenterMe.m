function [ zen ] = CenterMe( byteMe )
%CenterMe Centers the Zeros
%   Detailed explanation goes here
    [m,n] = size(byteMe);
    
    for i = 1:m
        for j = 1:n
            zen(i,j) = byteMe(i,j)*(-1)^(i+j);
        end
    end

end

