clear; close all; clc;

syms t1 t3 t5 t7 t9

trans = @(x) [1 0 0 x(1); 0 1 0 x(2); 0 0 1 x(3); 0 0 0 1];
r_x = @(theta) [1 0 0 0; 0 cosd(theta) -sind(theta) 0; 0 sind(theta) cosd(theta) 0; 0 0 0 1];
r_y = @(theta) [cosd(theta) 0 sind(theta) 0; 0 1 0 0; -sind(theta) 0 cosd(theta) 0; 0 0 0 1];
r_z = @(theta) [cosd(theta) -sind(theta) 0 0; sind(theta) cosd(theta) 0 0; 0 0 1 0; 0 0 0 1];


A1 = r_z(t1)*trans([0,0,7.34])*trans([0.619 0 0]);
A2 = r_z(-90)*trans([0 0 1.783]);
A3 = r_z(t3)*r_x(-90);
A4 = trans([0 0 10.2])*r_x(90);
A5 = r_z(t5)*r_x(-90);
A6 = trans([0 0 12.9])*r_x(90);
A7 = r_z(t7)*r_x(-90);
A8 = r_z(90)*trans([0 0 5.84])*r_x(90);
A9 = r_z(t9)*trans([0 0 11]);

T_0_9 = A1*A2*A3*A4*A5*A6*A7*A8*A9;