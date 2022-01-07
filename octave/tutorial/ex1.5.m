%excercise 1.5

x1 = linspace (-10 , 10 , 100)
x2 = linspace (-10, 10, 100);

plot(x, x1.^2.*sin(x), 'linewidth', 3, 'r', 
     x, x2.^2, 'k:',
     x, (-1*x2.^2), 'k:', 'linewidth', 2)
legend('y = x squared sin x','y = x squared','y = - x squared')
title("Plots")
grid on
