%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
 [x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
  %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\E82.wav');	% get some data 
  % [x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\110hz4kNoFilter.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\sine110Hz.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data

sampleBegin =100;
numOfSamples = 2048;
fftSize = 44100; % 2^14
%plot(x);
x=x(sampleBegin:((numOfSamples-1)+sampleBegin)); % select sample chunk 

w=hamming(numOfSamples);	%select a window
x=x.*w;		%window the signal


y=fft(x,fftSize);	%fourier transform, fft size = fftsize
y=circshift(y,-1);
Mag=abs(y);		%get the magnitude
Mag=Mag(1:(fftSize/2)); %omit redundant data from plot

Mag1 = downsample(Mag,2);
Mag2 = downsample(Mag,3);
Mag3 = downsample(Mag,4);
Mag4 = downsample(Mag,5);

subplot(5,1,1);
plot(Mag(1:1000));
grid;
xlabel('Hz');
ylabel('Mag');
title('FFT');

subplot(5,1,2);
plot(Mag1(1:1000));
grid;
xlabel('Hz');
ylabel('Mag');
title('FFT downsampled *2');

subplot(5,1,3);
plot(Mag2(1:1000));
grid;
xlabel('Hz');
ylabel('Mag');
title('FFT downsampled *3');

subplot(5,1,4);
plot(Mag3(1:1000));
grid;
xlabel('Hz');
ylabel('Mag');
title('FFT downsampled *4');

%{
subplot(5,1,5);
plot(Mag4(1:1000));
grid;
%}

for i=1:size(Mag1)
    
   hps(i) = Mag(i) * Mag1(i);
    
end


for i=1:size(Mag2)
    
   hps(i) = Mag(i) * Mag2(i);
    
end


for i=1:size(Mag3)
    
   hps(i) = Mag(i) * Mag3(i);
    
end


for i=1:size(Mag4)
    
   hps(i) = Mag(i) * Mag4(i);
    
end



subplot(5,1,5);
plot(hps(1:1000)/5);
grid;
title('Harmonic Spectrum Result');
xlabel('Hz');
ylabel('Mag');


