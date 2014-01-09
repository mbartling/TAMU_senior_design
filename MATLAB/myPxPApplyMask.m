function [ imOut ] = myPxPApplyMask( imIn, mask)
%% myPxPApplyMask zero pads an image an applys a mask
%   Note r = pixel in imIn
%   imIn = input image
%   mask = mask to apply
%   andNormalize = ask if want to divide by mask average (if simply summed
%   then would fail on the laplacian filters
%   appG = G(x,y) mask to original image

%% Get Sizes and offsets
[M, N] = size(imIn);
P = length(mask);
middle = (P+1)/2; %for P greater than 1
%padM = zeros(M,1);
%padN = zeros(1,N);

%% Begin ZeroPadded images
zpI = uint8(zeros((M+2*P),(N+2*P)) + min(min(imIn)));

% %populate with input image
zpI((P+1):(M+P),(P+1):(N+P)) = imIn;
norm = sum(sum(mask));    % precalc denominator
zpI = double(zpI);


%% Apply Linear Filter

    for i = (middle):(M+P)
        for j = (middle):(N+P)
            %element wise multiply with mask then sum2 to get numerator
            % denominator is known in advance
            %if (andNormalize) %some averaging filter
            zpI1(i+middle-1,j+middle-1) = sum(sum(zpI(i:(i+P-1), j:(j+P-1)).*mask))/norm;
            
            %else % Laplacian
            %    temp1 = zpI(i:(i+P-1), j:(j+P-1));
            %    temp = temp1.*mask;
            %    zpI(i+middle-1,j+middle-1) = sum(sum(temp));
            %end
        end
    end
    %% Crop Image
    zpI = zpI1((P+1):(M+P),(P+1):(N+P));

%imOut = uint8(zpI); %cast
imOut = zpI;



end

