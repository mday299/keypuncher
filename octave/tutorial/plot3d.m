x=linspace(-2,2,50);
y=linspace(-2,2,50);
[xx,yy]=meshgrid(x,y);
mesh(xx,yy,4-(xx.^2+yy.^2))