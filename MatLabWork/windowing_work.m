%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
%[x, Fs]=audioread('C:\Users\Donnacha\Downloads\files for spectrum\noteA.wav');
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\sine110Hz.wav');	% get some data 
 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data

mFactor = 0.1;
sampleBegin =512;
numOfSamples = 1024;
fftSize = 44100; % 2^14
x=x(sampleBegin:((numOfSamples-1)+sampleBegin)); % select sample chunk

subplot(5,1,1);
n = 0:1023;
plot(x);
y=fft(x,fftSize);		%fourier transform, fft size = fftsize
y=circshift(y,-1);
Mag=abs(y/(numOfSamples/2));		%get the magnitude
Mag=Mag(1:fftSize/2); %omit redundant data from plot
subplot(5,1,2);
plot(Mag);

w= (sin(pi*n/numOfSamples) - 0.3 *sin(3*pi*n/numOfSamples ) - 0.2 *sin(3*pi*n/numOfSamples ));

%{
for i=(numOfSamples/2):(numOfSamples-numOfSamples/2)
    w(i) = 1;
end
%}
subplot(5,1,3);
plot(w);

for i=1:numOfSamples
    x(i) = x(i)*w(i);
end


subplot(5,1,4);
plot(x);

y=fft(x,fftSize);		%fourier transform, fft size = fftsize
y=circshift(y,-1);
Mag=abs(y/(numOfSamples/2));		%get the magnitude
Mag=Mag(1:fftSize/2); %omit redundant data from plot
subplot(5,1,5);
plot(Mag);

%{
for i=1:numOfSamples/2
    
    x(i) = x(i)*mFactor;
    mFactor = mFactor+0.0009;
   
end

for i=numOfSamples/2+1:numOfSamples
    
    x(i) = x(i)*mFactor;
    mFactor = mFactor-0.0009;
   
end


%w=triang(numOfSamples);	%select a window
%x=x.*w;		%window the signal
subplot(4,1,3);
plot(x);
y=fft(x,fftSize);	%fourier transform, fft size = fftsize
y=circshift(y,-1);
Mag=abs(y/(numOfSamples/2));		%get the magnitude
Mag=Mag(1:fftSize/2); %omit redundant data from plot
subplot(4,1,4);
plot(Mag);
grid;
%}


