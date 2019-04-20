% -------------------------------------------------
% ------------ Interpolation of RGB ----------------
% ------------ Obiwahn_89 20.04.2019 ---------------
% -------------------------------------------------

% tabula rasa
clear all
close all
clc

% key parameters
time=[0,7,12,21];
dawn=[0.86,0.70,0.70];
day=[1,1,0.95];
dusk=[0.59,0.66,0.78];
night=[0.07,0.09,0.38];

% create a vector for each color (sample two days)
r=[night(1),dawn(1),day(1),dusk(1)];
r=[r,r,night(1)];
g=[night(2),dawn(2),day(2),dusk(2)];
g=[g,g,night(2)];
b=[night(3),dawn(3),day(3),dusk(3)];
b=[b,b,night(3)];
time=[time,time+24];
time=[time,time(length(time))+24-time(4)];


%% add data points to improve fit
% time vector [hour]
t=linspace(0,24);

% red
tdawn=time(2)-0.75;
tdusk=time(4)+0.75;
gaindawn = 0.5;
gaindusk = 0.5;
hr= 1-night(1);
fr = @(x) night(1)+0.5*hr.*(1+erf(gaindawn*(x-tdawn)))-(0.5*hr.*(1+erf(gaindusk*(x-tdusk))));
yr=fr(t);

% green
tdawn=time(2);
tdusk=time(4);
gaindawn = 0.5;
gaindusk = 0.5;
hg= 1-night(2);
fg = @(x) night(2)+0.5*hg.*(1+erf(gaindawn*(x-tdawn)))-(0.5*hg.*(1+erf(gaindusk*(x-tdusk))));
yg=fg(t);

% blue
tdawn=time(2);
tdusk=time(4)+0.75;
gaindawn = 0.5;
gaindusk = 0.5;
hb= day(3)-night(3);
fb = @(x) night(3)+0.5*hb.*(1+erf(gaindawn*(x-tdawn)))-(0.5*hb.*(1+erf(gaindusk*(x-tdusk))));
yb=fb(t);


%% fit Fourier functions

% use fourier to fit
fitr = fit(t',yr','fourier5');
fitg = fit(t',yg','fourier5');
fitb = fit(t',yb','fourier5');


%% plots
% plot the manual data VS desired colors
figure(1)
hold on
plot(t,yr,'--','Color','red','LineWidth',2)
plot(t,yg,'--','Color','green','LineWidth',2)
plot(t,yb,'--','Color','blue','LineWidth',2)
ylabel('magnitude of the color components')
xlabel('time of the day [h]')
set(gca,'FontSize',20)
set(gca,'color',[0.8,0.8,0.8])
xlim([0 24])
ylim([0 1.1])
title('Manual fit with error function')
grid on

% plot the manual data VS desired colors
figure(2)
plot(t,fitr(t),'-','Color','red','LineWidth',2)
hold on
plot(t,fitg(t),'-','Color','green','LineWidth',2)
plot(t,fitb(t),'-','Color','blue','LineWidth',2)
ylabel('magnitude of the color components')
xlabel('time of the day [h]')
set(gca,'FontSize',20)
set(gca,'color',[0.8,0.8,0.8])
xlim([0 24])
ylim([0 1.1])
title('Fourier approximation')
grid on


%% write out the coefficients
fileID = fopen('fitcoeff.txt','w');
fprintf(fileID,'a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w) + a4*cos(4*x*w) + b4*sin(4*x*w) + a5*cos(5*x*w) + b5*sin(5*x*w)\n');
fprintf(fileID,'var ra = [%f,%f,%f,%f,%f,%f]\n',fitr.a0,fitr.a1,fitr.a2,fitr.a3,fitr.a4,fitr.a5);
fprintf(fileID,'var rb = [%f,%f,%f,%f,%f]\n',fitr.b1,fitr.b2,fitr.b3,fitr.b4,fitr.b5);
fprintf(fileID,'var ga = [%f,%f,%f,%f,%f,%f]\n',fitg.a0,fitg.a1,fitg.a2,fitg.a3,fitg.a4,fitg.a5);
fprintf(fileID,'var gb = [%f,%f,%f,%f,%f]\n',fitg.b1,fitg.b2,fitg.b3,fitg.b4,fitg.b5);
fprintf(fileID,'var ba = [%f,%f,%f,%f,%f,%f]\n',fitb.a0,fitb.a1,fitb.a2,fitb.a3,fitb.a4,fitb.a5);
fprintf(fileID,'var bb = [%f,%f,%f,%f,%f]\n',fitb.b1,fitb.b2,fitb.b3,fitb.b4,fitb.b5);
fprintf(fileID,'var w = [%f,%f,%f]\n',fitr.w,fitg.w,fitb.w);
fclose(fileID);
    
