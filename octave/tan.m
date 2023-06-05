pkg load io
c = csv2cell('tan.csv',";");
s1 = cell2struct( c(2:end,:).', c(1,:) );
s2 = s1(1:32:end,:); #only select every 32nd row

clf;
hold on;

x = [s2.('x')]*pi;
y1 = [s2.('tan-flt')];
y2 = [s2.('tan-fp16')];


subplot (1, 2, 1)
plot(x,y1,'-','linewidth',1,x,y2,'-.','linewidth',2);

axis ([0, pi, -10, 10], "square");


subplot (1, 2, 2)
plot([s1.('x')]*pi,[s1.('errabs')]);
axis ([0, pi, 0 , 0.01], "square");


#legend ('floating point', 'fixed point');
