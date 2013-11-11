function [ out ] = middleIndex( myinput)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

    thelen = length(myinput);
    oddEven = mod(thelen,2);
    out = (thelen + oddEven)/2;


end

