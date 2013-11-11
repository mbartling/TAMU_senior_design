worldsize = 512;
maxsdev = 32;
stdev = rand(1)*randi(maxsdev);
world = stdev*randn(worldsize,worldsize);
anAnt = make_ant(16, 64, 64, 64);

%imshow(uint8(world))
imshow(uint8(anAnt))
%imhist(uint8(world))