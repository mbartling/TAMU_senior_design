close all;
example_maps

[xmax, ymax] = size(Ant_g1);

%number of samples to take
numPoints = 5;

% parameters
N           = 20;
sigma_x     = 5;
sigma_y     = 5;
scale       = 1;
normalize   = 0;
attenuation = 0.75;
% Pick arbitrary points for now
xCoord = randi(xmax, numPoints, 1);
yCoord = randi(ymax, numPoints, 1);

% s1 = zeros(xmax/2+1,ymax/2+1);
s1 = zeros(xmax,ymax);
s2 = zeros(xmax,ymax);
s3 = zeros(xmax,ymax);
s4 = zeros(xmax,ymax);


 for i = 1:numPoints
    s1(xCoord(i),yCoord(i)) = Ant_b(xCoord(i),yCoord(i));
    pts1(i,[1:3]) = [xCoord(i),yCoord(i), Ant_b(xCoord(i),yCoord(i))];
    s2(xCoord(i),yCoord(i)) = Ant_g1(xCoord(i),yCoord(i));
    s3(xCoord(i),yCoord(i)) = Ant_g2(xCoord(i),yCoord(i));
    s4(xCoord(i),yCoord(i)) = Ant_g3(xCoord(i),yCoord(i));
 end
 
 basicGauss = my2DGaussian(N,1,1,sigma_x,sigma_y,scale, 0, normalize);
 
%  figure; mesh(basicGauss);
 
 figure;
 mesh(-s1)
 %s5 = -s1;
 %s5 = fftshift(-s1);
 %si = fft2(s5);
 %mesh(abs(si))
 
 avg = sum(sum(-s1))/numPoints;
 zeroMask = -s1 == 0;
 s1 = -s1;
 pts1(:,3) = -pts1(:,3);
 s1(zeroMask) = avg;
 
 smax = max(max(s1)); %normalize
 s1 = s1./smax;
 
 pts1(:,3) = pts1(:,3)./smax;
 
 s1 = 1-s1;  % to reorient 
 pts1(:,3) = 1-pts1(:,3);
 
 figure; mesh(s1);
 
 %calculate euclidean distances
 for i=1:numPoints
    for j = 1:numPoints
        d1(i,j) = myDistance2(xCoord(i),yCoord(i), xCoord(j), yCoord(j)); 
        
    end
 end
 [xsize,ysize] = size(s1);
 DD = zeros(xsize,ysize,numPoints);
 for i = 1:numPoints
    temp = myDistance3(s1,pts1(i,1),pts1(i,2));
    DD(:,:,i) = temp;
 end
 %this average is not correct, however it gives us overlap
 avgdist = sum(sum(d1))/numel(d1);
 maxdist = max(max(d1));
 meddist = median(median(d1));
 d1m = d1 == 0;
 d11 = d1;
 d11(d1m) = max(max(d1)); % get rid of the zeros
 mindist = min(min(d11));
 
 D = myDistance3(s1,pts1(1,1),pts1(1,2));
 %create a basic circle
 radiusMask = D <= avgdist*attenuation;
 rmed = D <= meddist;
 rmin = D <= mindist;
 
 H = zeros(size(s1));
 Hmin = zeros(size(s1));
 Hmed = zeros(size(s1));
 
 H(radiusMask) = 1;
 Hmed(rmed) = 1;
 Hmin(rmin) = 1;
 
 newavg = sum(sum(s1))/numel(s1);
 nullspots = 0.003;
%  avgmask = abs(s1-newavg) < nullspots;
 
%  figure; imshow(H,[]);title('Havg');
%  figure; imshow(Hmed,[]); title('Hmed');
%  figure; imshow(Hmin,[]); title('Hmin');

H2 = zeros(size(s1));
 for i = 1:numPoints
   %  figure; hold on;
     H1 = zeros(size(s1));
     for j = 1:numPoints
         % note I could create a halo of a certain radius
        rmask = DD(:,:,i) <= d1(i,j);
        H = zeros(size(s1));   
        deltaRssi = pts1(i,3) - pts1(j,3);
        H(rmask) = 1;%*pts1(j,3);
        H = imfilter(H,basicGauss);
        H1 = H1 + H*deltaRssi;
   %     imshow(H1,[]);
     end
     H2 = H2 + H1;
  %   title(int2str(i))
  %   hold off;
 end
 figure; imshow(H2,[]); axis image; axis on;
 