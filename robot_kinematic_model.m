%clear; close all; clc;

syms t1 t3 t5 t7 t8

D1 = 7.34;
D2 = 1.783;
D6 = 10.2;
D10 = 12.9;
D14 = 5.84;
D15 = 11;

trans = @(x) [1 0 0 x(1); 0 1 0 x(2); 0 0 1 x(3); 0 0 0 1];
r_x = @(theta) [1 0 0 0; 0 cosd(theta) -sind(theta) 0; 0 sind(theta) cosd(theta) 0; 0 0 0 1];
r_y = @(theta) [cosd(theta) 0 sind(theta) 0; 0 1 0 0; -sind(theta) 0 cosd(theta) 0; 0 0 0 1];
r_z = @(theta) [cosd(theta) -sind(theta) 0 0; sind(theta) cosd(theta) 0 0; 0 0 1 0; 0 0 0 1];

A1 = r_z(t1)*trans([0 0 D1]);
A2 = trans([0 0 D2]);
A3 = r_x(-90);
A4 = r_z(t3);
A5 = r_x(90);
A6 = trans([0 0 D6]);
A7 = r_x(-90);
A8 = r_z(t5);
A9 = r_x(90);
A10 = trans([0 0 D10]);
A11 = r_x(-90);
A12 = r_z(t7);
A13 = r_x(90);
A14 = r_z(t8)*trans([0 0 D14]);
A15 = trans([0 0 D15]);

T_0_9 = A1*A2*A3*A4*A5*A6*A7*A8*A9*A10*A11*A12*A13*A14*A15;

B1 = r_z(t1)*trans([0 0 D1]);
B2 = trans([0 0 D2])*r_x(-90);
B3 = r_z(t3);
B4 = r_x(90);
B5 = trans([0 0 D6])*r_x(-90);
B6 = r_z(t5);
B7 = r_x(90);
B8 = trans([0 0 D10]) * r_x(-90);
B9 = r_z(t7);
B10 = r_x(90);
B11 = trans([0 0 D14])*r_z(t8);
B12 = trans([0 0 D15]);

T_0_12 = B1*B2*B3*B4*B5*B6*B7*B8*B9*B10*B11*B12;
