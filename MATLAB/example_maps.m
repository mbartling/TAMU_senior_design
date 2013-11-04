%%Example RF Field Map Generator
% Myles Foreman
% Fall 2013
% Currently generates isotropic radiation sources.

clear all;
%Variables
c = 300000000;
f = 2400000000;
lambda = c/f;
%End Variable

%Rx Variables
Rx_loss = 1;
Rx_mismatch = 1;
Rx_eff = Rx_loss*Rx_mismatch;
Rx_direc = 1.64;
%End Rx Variables

%World Map
world_size_row = 30;
world_size_column = 50;  %box of size 50 meters by 30 meters
world_division = 1/10;
world_matrix_size_row = world_size_row/world_division;
world_matrix_size_column = world_size_column/world_division;
world_map = zeros(world_matrix_size_row,world_matrix_size_column);
%matrix of world of 1000x1000 with divisions of .1 meters
%End world map

%First good antenna color blue
pt_g1 = 100;  %power transmitted in milliwatts
Tx_loss_g1 = 1;
Tx_mismatch_g1 = 1;
polar_eff_g1 = 1;
total_eff_g1 = Tx_loss_g1*Tx_mismatch_g1*polar_eff_g1*Rx_eff;
Tx_direc_g1 = 1.64;

row_g1 = randi(world_matrix_size_row);  %nth row from the bottom
column_g1 = randi(world_matrix_size_column);  %nth column from the left

Ant_g1 = make_ant_matrix(pt_g1,total_eff_g1,Rx_direc,Tx_direc_g1,world_matrix_size_row,world_matrix_size_column,world_division,row_g1,column_g1);
%end

%Second good antenna color green
pt_g2 = 100;  %power transmitted in milliwatts
Tx_loss_g2 = 1;
Tx_mismatch_g2 = 1;
polar_eff_g2 = 1;
total_eff_g2 = Tx_loss_g2*Tx_mismatch_g2*polar_eff_g2*Rx_eff;
Tx_direc_g2 = 1.64;

row_g2 = randi(world_matrix_size_row);  %nth row from the bottom
column_g2 = randi(world_matrix_size_column);  %nth column from the left

Ant_g2 = make_ant_matrix(pt_g2,total_eff_g2,Rx_direc,Tx_direc_g2,world_matrix_size_row,world_matrix_size_column,world_division,row_g2,column_g2);
%end

%Third good antenna color cyan
pt_g3 = 100;  %power transmitted in milliwatts
Tx_loss_g3 = 1;
Tx_mismatch_g3 = 1;
polar_eff_g3 = 1;
total_eff_g3 = Tx_loss_g3*Tx_mismatch_g3*polar_eff_g3*Rx_eff;
Tx_direc_g3 = 1.64;

row_g3 = randi(world_matrix_size_row);  %nth row from the bottom
column_g3 = randi(world_matrix_size_column);  %nth column from the left

Ant_g3 = make_ant_matrix(pt_g3,total_eff_g3,Rx_direc,Tx_direc_g3,world_matrix_size_row,world_matrix_size_column,world_division,row_g3,column_g3);
%end

%Bad antenna color red
pt_b = 100;  %power transmitted in milliwatts
Tx_loss_b = 1;
Tx_mismatch_b = 1;
polar_eff_b = 1;
total_eff_b = Tx_loss_b*Tx_mismatch_b*polar_eff_b*Rx_eff;
Tx_direc_b = 1.64;

row_b = randi(world_matrix_size_row);  %nth row from the bottom
column_b = randi(world_matrix_size_column);  %nth column from the left

Ant_b = make_ant_matrix(pt_b,total_eff_b,Rx_direc,Tx_direc_b,world_matrix_size_row,world_matrix_size_column,world_division,row_b,column_b);
%end

%contour line values for plotting
v_g1 = [round(10*log10(pt_g1*0.001)),round(10*log10(pt_g1*0.0001)),round(10*log10(pt_g1*0.00002)),round(10*log10(pt_g1*0.00001)),round(10*log10(pt_g1*0.000004)),round(10*log10(pt_g1*0.000002)),round(10*log10(pt_g1*0.000001)),round(10*log10(pt_g1*0.0000007)),round(10*log10(pt_g1*0.0000004)),round(10*log10(pt_g1*0.00000025)),round(10*log10(pt_g1*0.0000002)),round(10*log10(pt_g1*0.00000016)),round(10*log10(pt_g1*0.000000125)),round(10*log10(pt_g1*0.0000001))];
v_g2 = [round(10*log10(pt_g2*0.001)),round(10*log10(pt_g2*0.0001)),round(10*log10(pt_g2*0.00002)),round(10*log10(pt_g2*0.00001)),round(10*log10(pt_g2*0.000004)),round(10*log10(pt_g2*0.000002)),round(10*log10(pt_g2*0.000001)),round(10*log10(pt_g2*0.0000007)),round(10*log10(pt_g2*0.0000004)),round(10*log10(pt_g2*0.00000025)),round(10*log10(pt_g2*0.0000002)),round(10*log10(pt_g2*0.00000016)),round(10*log10(pt_g2*0.000000125)),round(10*log10(pt_g2*0.0000001))];
v_g3 = [round(10*log10(pt_g3*0.001)),round(10*log10(pt_g3*0.0001)),round(10*log10(pt_g3*0.00002)),round(10*log10(pt_g3*0.00001)),round(10*log10(pt_g3*0.000004)),round(10*log10(pt_g3*0.000002)),round(10*log10(pt_g3*0.000001)),round(10*log10(pt_g3*0.0000007)),round(10*log10(pt_g3*0.0000004)),round(10*log10(pt_g3*0.00000025)),round(10*log10(pt_g3*0.0000002)),round(10*log10(pt_g3*0.00000016)),round(10*log10(pt_g3*0.000000125)),round(10*log10(pt_g3*0.0000001))];
v_b =  [round(10*log10(pt_b*0.001)),round(10*log10(pt_b*0.0001)),round(10*log10(pt_b*0.00002)),round(10*log10(pt_b*0.00001)),round(10*log10(pt_b*0.000004)),round(10*log10(pt_b*0.000002)),round(10*log10(pt_b*0.000001)),round(10*log10(pt_b*0.0000007)),round(10*log10(pt_b*0.0000004)),round(10*log10(pt_b*0.00000025)),round(10*log10(pt_b*0.0000002)),round(10*log10(pt_b*0.00000016)),round(10*log10(pt_b*0.000000125)),round(10*log10(pt_b*0.0000001))];
%end

%plotting
contour(Ant_g1,'b','ShowText','on');
hold on
contour(Ant_g2,'g','ShowText','on');
contour(Ant_g3,'c','ShowText','on');
contour(Ant_b,'r','ShowText','on');
xlabel('0.1 meters per division');
ylabel('0.1 meters per division')
hold off
%end
