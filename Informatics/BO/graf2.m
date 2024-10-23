%clear all;
clf;
v = -2:0.005:8;  % plotting range from -5 to 5
[x1,x2] = meshgrid(v);  % get 2-D mesh for x and y
bazowe = [0, 0; 3, 0; 6, 0; 0, 2; 0, 3; -6, 6];
cond1 = 2*x1+4*x2 <= 12;  % check conditions for these values
cond2 = x1+4*x2 <= 7;
cond3 = 2*x1+5*x2 <= 12;
cond4 = x1+3*x2 <= 6;
condGeq = x1 >= 0 & x2 >= 0;
cond1 = double(cond1);  % convert to double for plotting
cond2 = double(cond2);
cond3 = double(cond3);
cond4 = double(cond4);
condGeq = double(condGeq);
cond1(cond1 == 0) = NaN;  % set the 0s to NaN so they are not plotted
cond2(cond2 == 0) = NaN;
cond3(cond3 == 0) = NaN;
cond4(cond4 == 0) = NaN;
condGeq(condGeq == 0) = NaN;
cond = cond1.*cond2.*cond3.*cond4.*condGeq;  % multiply the two condaces to keep only the common points

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
C(:,:,2) = ones(len);  % green
C(:,:,3) = zeros(len);   % blue
s = surf(x1,x2,cond3, C, 'FaceAlpha',0.5);
s.EdgeColor = 'none';
C(:,:,1) = ones(len).*0.25;% red
C(:,:,2) = ones(len).*0.75;  % green
C(:,:,3) = ones(len);   % blue
s = surf(x1,x2,cond4, C, 'FaceAlpha',0.5);
s.EdgeColor = 'none';
C(:,:,1) = ones(len);  % red
C(:,:,2) = zeros(len);  % green
C(:,:,3) = ones(len);   % blue
%s = surf(x1,x2,cond, C);
%s.EdgeColor = 'none';
%scatter3(bazowe(1, 1), bazowe(1, 2), 1, 'filled');
%scatter3(bazowe(2, 1), bazowe(2, 2), 1, 'filled');
%scatter3(bazowe(3, 1), bazowe(3, 2), 1, 'filled');
%scatter3(bazowe(4, 1), bazowe(4, 2), 1, 'filled');
%scatter3(bazowe(5, 1), bazowe(5, 2), 1, 'filled');
%scatter3(bazowe(6, 1), bazowe(6, 2), 1, 'filled');
view(2);
%legend("x_1,x_2 \geq 0","x_1+2x_2\geq6","2x_1+3x_2\leq6", "I", "II", "III", "IV", "V", "VI");
wspkierunkowy = 14/6;
przesuniecie = 0;
if (wspkierunkowy > 0)
    minv = min(v);
    maxv = max(v);
else
    minv = -max(v);
    maxv = -min(v);
end
%if (abs(wspkierunkowy) >= 1)
    %line([minv+przesuniecie,maxv]./wspkierunkowy, [min(v),max(v)- przesuniecie], [1,1], 'Color','red','LineStyle','--', 'LineWidth', 2)
%else
    %line([min(v),max(v)], [minv,maxv].*wspkierunkowy-przesuniecie, [1,1], 'Color','red','LineStyle','--', 'LineWidth', 2)
%end
scatter3(6, 0, 1, 'filled');
%legend("x_1,x_2 \geq 0","x_1+x_2\geq4","-2x_1+3x_2\leq6", "izokwanta");
legend("x_1,x_2\geq0","2x_1+4x_2\leq12","x_1+4x_2\leq7", "2x_1+5x_2\leq12", "x_1+3x_2\leq6", "rozwiazanie (6;0)");
xlabel("x_1");
ylabel("x_2");
