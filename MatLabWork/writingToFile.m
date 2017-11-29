%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)

 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data
[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\sine110Hz.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data



sampleBegin =512;
numOfSamples = 1024;
fftSize = 44100; % 2^14

x=x(sampleBegin:((numOfSamples-1)+sampleBegin)); % select sample chunk

fid=fopen('C:\Users\Donnacha\Desktop\Final-Project\MatLabWork\guitardata100HzSineWave.txt','w');
fprintf(fid, ' Guitar Note D 292Hz time domain data --1024 samples-- \r\n');
fprintf(fid, '%f \r\n',x');
fclose(fid);