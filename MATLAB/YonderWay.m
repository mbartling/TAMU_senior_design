function [ TreasureMap ] = YonderWay( ui_start, ui_end, mapMe, minRSSI, myParams )
%% YonderWay
% ui_start = user input coordinate to start at
% ui_end   = user input coordinate to end with
% mapMe    = playing field

%% Initialize variables

alpha = myParams.alpha;
geometricR = myParams.geometricR;
numRuns = myParams.numRuns;
pathNoiseMax = myParams.pathNoiseMax;
%myParams.visualizeMode;

% alpha   = 20;
% geometricR = 0.8;
% numRuns = 10;
% pathNoiseMax = 10;
%%Note Right now the Sample Rate doesn't work properly.
sampleRate = 1;
%%
% Get the initial Line to follow
x1 = ui_start.x;
y1 = ui_start.y;
x2 = ui_end.x;
y2 = ui_end.y;
dx = x2-x1;
dy = y2-y1;
[sizeX, sizeY] = size(mapMe);
%seglength = sqrt((dx)^2 + (dy)^2); % unused

[path.x, path.y] = bresenham(x1,y1,x2,y2); % Computer graphics line maker
[invpath.x, invpath.y] = bresenham(x1,y2,x2,y1); %Calculate the inverse since this will approximate normal without changing
ming1 = min(min(mapMe));
TreasureMap.fogOfWar = zeros(size(mapMe))+min(min(mapMe));
TreasureMap.path2.x = path.x;
TreasureMap.path2.y = path.y;
newThresh = 0.95*minRSSI;   % may implement later
numPoints = length(path.x); % These should have the same number of elements


%% Add some path Noise

pathNoisex = randi(pathNoiseMax,size(path.x));
pathNoisey = randi(pathNoiseMax,size(path.y));
pathNoisex = pathNoisex - round(pathNoiseMax/2);
pathNoisey = pathNoisey - round(pathNoiseMax/2);
path.x = path.x + pathNoisex;
path.y = path.y + pathNoisey;


% figure
% contour(mapMe,'b','ShowText','on');
% hold on

%% Make sure noise doesnt go out of bounds
for i = 1:length(path.x)
    xx = path.x(i);
    yy = path.y(i);
    
    if( xx < 1 || xx > sizeX || yy < 1 || yy > sizeY)
        %%
        % instead of removing the location, which sucks in matlab, just
        % replace the out of bounds coordinate with the known
        % point
        path.x(i) = path.x(i) - pathNoisex(i);
        path.y(i) = path.y(i) - pathNoisey(i);
    else
        path.x(i) = path.x(i);
        path.y(i) = path.y(i);
    end
    
end

%% Get initial path values
for i = 1:sampleRate:numPoints
    sampledPoint = mapMe(path.x(i),path.y(i));
    if( sampledPoint > minRSSI ) %Mark as active Antenna
        TreasureMap.fogOfWar(path.x(i),path.y(i)) = sampledPoint;
        TreasureMap.path1.samples(i) = sampledPoint;
        TreasureMap.path1.x(i) = path.x(i);
        TreasureMap.path1.y(i) = path.y(i);
    else
        %Note Need to update this from g1 to whatever the active map is
        %TreasureMap.fogOfWar(path.x(i),path.y(i)) = ming1;
        TreasureMap.fogOfWar(path.x(i),path.y(i)) = sampledPoint;
        TreasureMap.path1.samples(i) = ming1;
        TreasureMap.path1.x(i) = path.x(i);
        TreasureMap.path1.y(i) = path.y(i);
    end
end
% hold off
TreasureMap.path1.x = TreasureMap.path1.x';
TreasureMap.path1.y = TreasureMap.path1.y';
TreasureMap.path1.samples = TreasureMap.path1.samples';
%% Find Max along path
locMax.val = max(TreasureMap.path1.samples);
loci = find(TreasureMap.path1.samples == locMax.val,1); %get the indices
locMax.locx = TreasureMap.path1.x(loci);
locMax.locy = TreasureMap.path1.y(loci);

%slope_f = -slope_f;
%% Shorten Paths and scoot

%invpath.x = invpath.x(alpha:length(invpath.x)-alpha) ;
%invpath.y = invpath.y(alpha:length(invpath.y)-alpha) ;
%% NOTE THIS IS CHANGED
% This rotation matrix ignores the 90 degree rotation, it is basically used
% just to send the quadcopter in the right direction based on the current
% path maximum and the previous maximum. This is done only at the end of a
% path run
theta = atan2((locMax.locy),(locMax.locx));
invpath.x =    round(invpath.x + locMax.locx*cos(theta)/2);
invpath.y =    round(invpath.y + locMax.locy*sin(theta)/2);
path.x    =    round(path.x    + locMax.locx*cos(theta)/2);
path.y    =    round(path.y    + locMax.locy*sin(theta)/2);
path.x = path.x(alpha:(length(path.x)-alpha));
path.y = path.y(alpha:(length(path.y)-alpha));



[path.x, path.y] = bresenham(path.x(1),path.y(1),path.x(end),path.y(end)); % Computer graphics line maker
%Calculate the inverse since this will approximate normal without changing
[invpath.x, invpath.y] = bresenham(invpath.x(1),invpath.y(1),invpath.x(end),invpath.y(end));



%% Add path Noise

pathNoisex = randi(pathNoiseMax,size(invpath.x));
pathNoisey = randi(pathNoiseMax,size(invpath.y));
pathNoisex = pathNoisex - round(pathNoiseMax/2);
pathNoisey = pathNoisey - round(pathNoiseMax/2);
invpath.x = invpath.x + pathNoisex;
invpath.y = invpath.y + pathNoisey;
%TreasureMap.path2.x = invpath.x + locMax.locx;
%TreasureMap.path2.y = invpath.y + locMax.locy;
TreasureMap.path2.x = invpath.x;
TreasureMap.path2.y = invpath.y;
dx2 = round(dx/2);
dy2 = round(dy/2);
x1 = x1 - alpha;
x2 = x2 - alpha;
TreasureMap.locMax = locMax;

isodd = 1;
%%
if (myParams.visualizeMode == 1)
    figure
    hold on
    scatter(TreasureMap.path1.y, TreasureMap.path1.x);
end
%%
for j = 2:numRuns
    alpha = round(geometricR*alpha);
    TreasureMap.path1.samples = [];
    %%
    if myParams.visualizeMode == 1
        scatter(TreasureMap.path2.y, TreasureMap.path2.x);
    end
    
    k = mod(isodd,2); % is k odd or even
    for i = 1:length(TreasureMap.path2.x)
        xx = TreasureMap.path2.x(i);
        yy = TreasureMap.path2.y(i);
        
        if( xx < 1 || xx > sizeX || yy < 1 || yy > sizeY)
            %%
            % instead of removing the location, which sucks in matlab, just
            % replace the out of bounds coordinate with the known
            % max coordinate
            TreasureMap.path2.x(i) = TreasureMap.locMax.locx;
            TreasureMap.path2.y(i) = TreasureMap.locMax.locy;
        else
            TreasureMap.path2.x(i) = TreasureMap.path2.x(i);
            TreasureMap.path2.y(i) = TreasureMap.path2.y(i);
        end
        
    end
    for i = 1:sampleRate:length(TreasureMap.path2.x)
        sampledPoint = mapMe(TreasureMap.path2.x(i),TreasureMap.path2.y(i));
        if( sampledPoint > minRSSI ) %Mark as active Antenna
            TreasureMap.fogOfWar(TreasureMap.path2.x(i),TreasureMap.path2.y(i)) = sampledPoint;
            TreasureMap.path1.samples(i) = sampledPoint;
            TreasureMap.path1.x(i) = TreasureMap.path2.x(i);
            TreasureMap.path1.y(i) = TreasureMap.path2.y(i);
        else
            %Note Need to update this from g1 to whatever the active map is
            TreasureMap.fogOfWar(TreasureMap.path2.x(i),TreasureMap.path2.y(i)) = ming1;
            TreasureMap.path1.samples(i) = ming1;
            TreasureMap.path1.x(i) = TreasureMap.path2.x(i);
            TreasureMap.path1.y(i) = TreasureMap.path2.y(i);
        end
    end
    
    
    %% Find Max along path
    oldlocMax = TreasureMap.locMax;
    locMax.val = max(TreasureMap.path1.samples);
    tempcount = 0;
    for i = 1:length(TreasureMap.path1.samples)
        if(TreasureMap.path1.samples(i) == locMax.val)
            tempcount = tempcount + 1;
            loci(tempcount) = i;
            %break;
        end
    end
    tempi = randi(tempcount,1); %the possible indices
    loci = loci(tempi);
    %loci = find(TreasureMap.path1.samples == locMax.val,1); %get the indices
    locMax.locx = TreasureMap.path1.x(loci);
    locMax.locy = TreasureMap.path1.y(loci);
    
    %path.x = TreasureMap.path1.x(loci);
    %path.y = TreasureMap.path1.y(loci);
    
    %% Shorten Paths and scoot
    %alpha = 10;
    %     invpath.x = invpath.x(alpha:length(invpath.x)-alpha);
    %     invpath.y = invpath.y(alpha:length(invpath.y)-alpha);
    %     path.x = path.x(alpha:length(path.x)-alpha);
    %     path.y = path.y(alpha:length(path.y)-alpha);
    temp.x = path.x(alpha:(length(path.x)-alpha));
    temp.y = path.y(alpha:(length(path.y)-alpha));
    
    %     path.x = invpath.x(alpha:length(invpath.x)-alpha)+ locMax.locx;
    %     path.y = invpath.y(alpha:length(invpath.y)-alpha)+ locMax.locy;
    path.x = invpath.x(alpha:(length(invpath.x)-alpha));
    path.y = invpath.y(alpha:(length(invpath.y)-alpha));
    invpath.x = temp.x ;
    invpath.y = temp.y ;
    
    [path.x, path.y] = bresenham(path.x(1),path.y(1),path.x(length(path.x)),path.y(length(path.y)));
    % Computer graphics line maker
    %Calculate the inverse since this will approximate normal without changing
    [invpath.x, invpath.y] = bresenham(invpath.x(1),invpath.y(1),invpath.x(length(invpath.x)),invpath.y(length(invpath.y)));
    
    
    %% Add path Noise
    
    pathNoisex = randi(pathNoiseMax,size(invpath.x));
    pathNoisey = randi(pathNoiseMax,size(invpath.y));
    pathNoisex = pathNoisex - round(pathNoiseMax/2);
    pathNoisey = pathNoisey - round(pathNoiseMax/2);
    invpath.x = invpath.x + pathNoisex;
    invpath.y = invpath.y + pathNoisey;
    
    %% Go Yonderway
    % This rotation matrix ignores the 90 degree rotation, it is basically used
    % just to send the quadcopter in the right direction based on the current
    % path maximum and the previous maximum. This is done only at the end of a
    % path run
    theta = atan2((locMax.locy - oldlocMax.locy),(locMax.locx - oldlocMax.locx));
    TreasureMap.path2.x = round(invpath.x + locMax.locx * cos(theta)/2);
    TreasureMap.path2.y = round(invpath.y + locMax.locy * sin(theta)/2);
    TreasureMap.path1.x = round(path.x    + locMax.locx * cos(theta)/2);
    TreasureMap.path1.y = round(path.y    + locMax.locy * sin(theta)/2);
    
    %TreasureMap.path2.x = round(TreasureMap.path2.x*(max(TreasureMap.path2.x)/ui_end.x));
    %TreasureMap.path2.y = round(TreasureMap.path2.y*(max(TreasureMap.path2.y)/ui_end.y));
    TreasureMap.locMax = locMax;
    isodd = isodd+1;
    
    
end
%%
if myParams.visualizeMode == 1
    hold off
    TreasureMap.Funstuff = 'Hi there';
end
