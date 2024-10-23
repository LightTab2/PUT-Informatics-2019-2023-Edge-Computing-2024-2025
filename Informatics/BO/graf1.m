%clear all;
clf;
v = -2:0.005:10;  % plotting range from -5 to 5
[x1,x2] = meshgrid(v);  % get 2-D mesh for x and y
bazowe = [0, 0; 2, 0; 4, 0; 0, 8; 0, 4; 4/3, 8/3];
cond1 = x1+x2 >= 4;  % check conditions for these values
cond2 = 4*x1+x2 <= 8;
condGeq = x1 >= 0 & x2 >= 0;
cond1 = double(cond1);  % convert to double for plotting
cond2 = double(cond2);
condGeq = double(condGeq);
cond1(cond1 == 0) = NaN;  % set the 0s to NaN so they are not plotted
cond2(cond2 == 0) = NaN;
condGeq(condGeq == 0) = NaN;
cond = cond1.*cond2.*condGeq;  % multiply the two condaces to keep only the common points

len = length(v);
hold on;
C(:,:,1) = zeros(len);  % red
C(:,:,2) = ones(len);  % green
C(:,:,3) = zeros(len);   % blue
s = surf(x1,x2,condGeq, C, 'FaceAlpha',0.5);
s.EdgeColor = 'none';
C(:,:,1) = zeros(len);  % red
C(:,:,2) = zeros(len);  % green
C(:,:,3) = ones(len);   % blue
s = surf(x1,x2,cond1, C, 'FaceAlpha',0.5);
s.EdgeColor = 'none';
C(:,:,1) = ones(len);  % red
C(:,:,2) = zeros(len);  % green
C(:,:,3) = zeros(len);   % blue
s = surf(x1,x2,cond2, C, 'FaceAlpha',0.5);
s.EdgeColor = 'none';
C(:,:,1) = ones(len);  % red
C(:,:,2) = zeros(len);  % green
C(:,:,3) = ones(len);   % blue
s = surf(x1,x2,cond, C);
s.EdgeColor = 'none';
scatter3(bazowe(1, 1), bazowe(1, 2), 1, 'filled');
scatter3(bazowe(2, 1), bazowe(2, 2), 1, 'filled');
scatter3(bazowe(3, 1), bazowe(3, 2), 1, 'filled');
scatter3(bazowe(4, 1), bazowe(4, 2), 1, 'filled');
scatter3(bazowe(5, 1), bazowe(5, 2), 1, 'filled');
scatter3(bazowe(6, 1), bazowe(6, 2), 1, 'filled');
view(0, 90);
legend("x_1,x_2 \geq 0","x_1+x_2\geq4","4x_1+x_2\leq8","Zbior rozwiazan dopuszczalnych", "I", "II", "III", "IV", "V", "VI");