function [ staticMap ] = StaticAlgorithmFunc( sample, ptsample, holder )
%Static Processing Does each one separately for now (not efficient but
%easier in matlab
%   Detailed explanation goes here
 numPoints  = holder.numPoints;
 xCoord     = holder.xCoord;
 yCoord     = holder.yCoord;
 basicGauss = holder.basicGauss;
 
 avg = sum(sum(-sample))/numPoints;
 zeroMask = -sample == 0;
 sample = -sample;
 ptsample(:,3) = -ptsample(:,3);
 sample(zeroMask) = avg;
 
 smax = max(max(sample)); %normalize
 sample = sample./smax;
 
 ptsample(:,3) = ptsample(:,3)./smax;
 
 sample = 1-sample;  % to reorient 
 ptsample(:,3) = 1-ptsample(:,3);
 
 %%figure; mesh(sample);
 
 %calculate euclidean distances
 for i=1:numPoints
    for j = 1:numPoints
        d1(i,j) = myDistance2(xCoord(i),yCoord(i), xCoord(j), yCoord(j)); 
        
    end
 end
 [xsize,ysize] = size(sample);
 DD = zeros(xsize,ysize,numPoints);
 for i = 1:numPoints
    temp = myDistance3(sample,ptsample(i,1),ptsample(i,2));
    DD(:,:,i) = temp;
 end
 %this average is not correct, however it gives us overlap
%  avgdist = sum(sum(d1))/numel(d1);
%  maxdist = max(max(d1));
%  meddist = median(median(d1));
%  d1m = d1 == 0;
%  d11 = d1;
%  d11(d1m) = max(max(d1)); % get rid of the zeros
%  mindist = min(min(d11));
%  
%  D = myDistance3(sample,ptsample(1,1),ptsample(1,2));
%  %create a basic circle
%  radiusMask = D <= avgdist*attenuation;
%  rmed = D <= meddist;
%  rmin = D <= mindist;
%  
%  H = zeros(size(sample));
%  Hmin = zeros(size(sample));
%  Hmed = zeros(size(sample));
%  
%  H(radiusMask) = 1;
%  Hmed(rmed) = 1;
%  Hmin(rmin) = 1;
%  
%  newavg = sum(sum(sample))/numel(sample);
%  nullspots = 0.003;
%  avgmask = abs(sample-newavg) < nullspots;
 
%  %figure; imshow(H,[]);title('Havg');
%  %figure; imshow(Hmed,[]); title('Hmed');
%  %figure; imshow(Hmin,[]); title('Hmin');

H2 = zeros(size(sample));
 for i = 1:numPoints
   %  %figure; hold on;
     H1 = zeros(size(sample));
     for j = 1:numPoints
         % note I could create a halo of a certain radius
        rmask = DD(:,:,i) <= d1(i,j);
        H = zeros(size(sample));   
        deltaRssi = ptsample(i,3) - ptsample(j,3);
        H(rmask) = 1;%*ptsample(j,3);
        H = imfilter(H,basicGauss);
        H1 = H1 + H*deltaRssi;
   %     imshow(H1,[]);
     end
     H2 = H2 + H1;
  %   title(int2str(i))
  %   hold off;
 end

 %return the staic map
staticMap = H2;
end

