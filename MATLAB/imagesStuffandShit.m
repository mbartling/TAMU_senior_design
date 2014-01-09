imIn = imread('coins.png');
fftImage = fft2(imIn);
fftImage = fftshift(fftImage);
close all
figure
imshow(imIn);

figure
Xr = real(fftImage);
imshow(Xr);
colormap(jet); colorbar

figure
Xi = imag(fftImage);
imshow(Xi);
colormap(jet); colorbar