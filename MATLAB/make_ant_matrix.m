%% Make Antenna Matrix
% - Myles Foreman
% - Fall 2013
% - Makes an isotropic antenna matrix using Friis Transmission Formula


function [ ant_matrix ] = make_ant_matrix( pt , total_eff , Rx_direc , Tx_direc , world_size_row , world_size_column , world_division , row_position , column_position )

%Variables
freq = 2400000000;
c = 300000000;
lambda = c/freq;
%End Variables

%initial matrix size
distance = world_division;
size_row = world_size_row*2-1;
size_column = world_size_column*2-1;
temp_matrix = zeros(size_row,size_column);
%end

%Friis equation excluding distance "r" from antenna
a = 10*log10(pt);  %pt in dBm
b = 10*log10(total_eff);  %effeciency in dB
c = 10*log10((lambda/(4*pi))^2);  %free space loss factor without "r"
d = 10*log10(Rx_direc) + 10*log10(Tx_direc);  %directivity of Rx and Rx in dB
e = a + b + c + d;  %all but "r" in dB
%end

%make a matrix to put into the world map matrix
for i = -(world_size_row-1):world_size_row-1
    for j = -(world_size_column-1):world_size_column-1
        if i^2 + j^2 ~= 0
            r_power2 = (distance*i)^2 + (distance*j)^2;  %distance from center
        else
            r_power2 = (distance*0.5)^2;  %for monotonic(ish) decent
        end
        f = 10*log10(1/r_power2);
        power_received = e + f;  %power received in dBm
        temp_matrix(i+world_size_row,j+world_size_column) = power_received;
    end
end
%end

%place temp matrix into the world map matrix at a specific position
ant_matrix = temp_matrix(world_size_row+1-row_position:2*world_size_row-row_position,world_size_column+1-column_position:2*world_size_column-column_position);
%end

end
