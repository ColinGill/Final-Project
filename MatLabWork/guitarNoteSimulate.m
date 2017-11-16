Fs = 44100;
n =1:44100;
x = sin(0.9*pi*(110/Fs)*n)+ 1.6*sin(2*pi*(220/Fs)*n)+ 2.2*sin(2*pi*(330/Fs)*n)+ 2.1*sin(2*pi*(440/Fs)*n) + 1.8*sin(2*pi*(550/Fs)*n) + 0.9*sin(2*pi*(660/Fs)*n) + 0.5*sin(2*pi*(770/Fs)*n);  
plot(x);

[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_A_110Hz_1sec.wav');
%plot(x1);