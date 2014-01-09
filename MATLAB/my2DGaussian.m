function [ GaussOut ] = my2DGaussian( N, mu_x, mu_y, sigma_x, sigma_y ,scale ,withRound, normalize)
%% MY2DGAUSSIAN generates a NxN 2D gaussian matrix for filtering
% Very simple
%%
%   Michael Bartling
%   ECEN 447
%
% just going to apply the 2D gaussian equation 
%% The big 2D Gaussian Equation
% $$f(x,y) = \frac{1}{2\pi\sigma_x\sigma_y}e^{-((x-\mu_x)^2/(2\sigma_x^2)+(y-\mu_y)^2/(2\sigma_y^2))}$$
%
%% The Easy 2D Gaussian Equation
% Currently, $\mu_x = \mu_y = 0$ and $\sigma_y$ is useless
GaussOut = ones(N);

%% 
% Do the function using the simple Gaussian filter from the slides
% $$ G(x,y)= \frac{1}{2\pi\sigma^2}e^{-\frac{x^2+y^2}{2\sigma^2})} $$
%
% nhalf = (N-1)/2;
% for x = -nhalf:nhalf %1:N
%     for y = -nhalf:nhalf %1:N
%         % leave support for this later
%         GaussOut(x+nhalf+1,y+nhalf+1) = 1/(2*pi*sigma_x*sigma_y)*exp(-((x-mu_x)^2/(2*sigma_x^2) + (y-mu_y)^2/(2*sigma_y^2)));
%         %GaussOut(x+nhalf+1,y+nhalf+1) = (1/(2*pi*sigma_x^2))*exp(-((x)^2 + (y)^2)/(2*sigma_x^2));
%     end
%     
% end
nhalf = (N-1)/2+1; % to zero the center
for x = 1:N
    for y = 1:N
        % leave support for this later
        GaussOut(x,y) = exp(-((x-nhalf)^2/(2*sigma_x^2) + (y-nhalf)^2/(2*sigma_y^2)))/(2*pi*sigma_x*sigma_y);
        %GaussOut(x+nhalf+1,y+nhalf+1) = (1/(2*pi*sigma_x^2))*exp(-((x)^2 + (y)^2)/(2*sigma_x^2));
    end
    
end
%% 
% Ask if user wants to round the solution up
if (normalize)
    avg = sum(sum(GaussOut))/(N*N);
    GaussOut = GaussOut./avg;
    if (withRound)
       GaussOut = round(scale*GaussOut);
    else
       GaussOut = scale*GaussOut;   
    end
else
    if (withRound)
        GaussOut = round(scale*GaussOut);
    else
       GaussOut = scale*GaussOut;   
    end
end

 
%GaussOut = round(scale*(GaussOut/sum(sum(GaussOut))));

end

