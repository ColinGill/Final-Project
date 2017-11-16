%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_F_88Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_E_82Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_A_110Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Octave_D_292Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\High_F_695Hz_1sec.wav');	% get some data

Fs = 2048;
n =1:2048;
x = sin(2*pi*(9/Fs)*n)+ 1/2*sin(2*pi*(18/Fs)*n);
plot(x);
max = max(x)
rmsValue =sqrt((( 0.7071 *1)^2))
y=fft(x,Fs);		%fourier transform, fft size = 44100
Mag=abs(y/(Fs/2));		%get the magnitude
Mag=Mag(1:Fs/2); %omit redundant data from plot
%fftMax = max(Mag);
plot(Mag);
fftRms = (1 *sqrt(2))
grid;


