function [ frame_size ] = calcFrameLength( Frame)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
frame_size = sum(structfun(@(field)length(field), Frame));


end

