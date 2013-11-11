function [ TreasureMap ] = stripOutofBounds( TreasureMapin )
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
for i = 1:length(TreasureMapin.path2.x)
    xx = TreasureMapin.path2.x(i);
    yy = TreasureMapin.path2.y(i);
    
    if( xx < 1 || xx > sizeX || yy < 1 || yy > sizeY)
        %%
        % instead of removing the location, which sucks in matlab, just
        % replace the out of bounds coordinate with the known
        % max coordinate
        TreasureMap.path2.x(i) = TreasureMapin.locMax.locx;
        TreasureMap.path2.y(i) = TreasureMapin.locMax.locy;
    else
        TreasureMap.path2.x(i) = TreasureMapin.path2.x(i);
        TreasureMap.path2.y(i) = TreasureMapin.path2.y(i);
    end
    
end

end

