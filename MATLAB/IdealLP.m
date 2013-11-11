function [ imOut ] = IdealLP( imIn, threshold )
%UNTITLED6 Summary of this function goes here
%   Simple logical indexing
    D = myDistance(imIn);
    %maskZero = D > threshold;
    % For logical indexing
    maskOne  = D <= threshold;
    
    H = zeros(size(imIn));
    % Set all indexes that are less than the threshold radius to one
    H(maskOne) = 1;
    
    %% Apply Mask, simply term by term multiplication
    imOut.F = H.*imIn;
    imOut.H = H;
    imOut.HP = (1-H);
    imOut.HPF = (1-H).*imIn;
end

