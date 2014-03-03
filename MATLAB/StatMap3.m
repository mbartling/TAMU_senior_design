close all;
clear all;
load('antb.mat');
xsamples = 50:50:250;
ysamples = 50:50:450;
numsamples = 3;
xsamples = randi(300,numsamples);
ysamples = randi(500,numsamples);
mask = zeros(size(Ant_b));
mask(xsamples,ysamples) = 1;

vals = Ant_b(logical(mask));
% vals = 10.^(10*Ant_b(logical(mask)));

[xloc,yloc] = ind2sub(size(Ant_b),find(mask));
xmean = mean(xloc);
ymean = mean(yloc);

l1 = (xloc-xmean)'*vals;
l2 = (yloc -ymean)'*vals;
l3 = sum(vals);

x2 = (xloc-xmean)'*(xloc-xmean);
y2 = (yloc-ymean)'*(yloc-ymean);
xy = (xloc-xmean)'*(yloc-ymean);

n = numel(xloc);
amat = [x2 xy 0; xy y2 0; 0 0 n];
L = [l1;l2;l3];
ainv = amat^-1;

coefmat = ainv*L;
alpha = coefmat(1);
beta = coefmat(2);
gamemap = zeros(size(Ant_b));
for i = 1:300
    for j = 1:500
        gamemap(i,j) = [(i-xmean) (j-ymean) 1]*coefmat;
    end
end

gamemap = gamemap/(sum(sum(gamemap))/numel(gamemap));
figure; contour(gamemap);
figure; contour(Ant_b);

xs = xloc-xmean;
ys = yloc-ymean;

amat = ones(numel(xs),5);
amat(:,1) = xs.^2;
amat(:,2) = ys.^2;
amat(:,3) = xs.^1;
amat(:,4) = ys.^1;
amat(:,5) = 1;


amatPI = amat'*amat;
coefmat = amatPI^-1*amat'*vals;

gamemap = zeros(size(Ant_b));
for i = 1:300
    for j = 1:500
        gamemap(i,j) = [(i-xmean)^2 (j-ymean)^2 (i-xmean)^1 (j-ymean)^1 1]*coefmat;
        
    end
end
gamemap = gamemap/(sum(sum(gamemap))/numel(gamemap));

figure;
subplot(1,2,1); contour(gamemap);
subplot(1,2,2); contour(Ant_b);

% mask = fspecial('laplacian');
% gamemap = imfilter(gamemap, mask);
gamemap = -log((alpha^2+beta^2)*(gamemap));
figure; contour(Ant_b);
figure; contour(gamemap);


