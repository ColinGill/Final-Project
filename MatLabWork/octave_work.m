clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data 
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Asinewave.wav');	% get some data 

audio = x1(1:1024);
x=x1(1:1024);		% select the sample frame, size =881 (20 ms)
n=1;

for i=1:1024
   
    remainder = mod(i,2);
  if (remainder == 1)
      y(n)=x(i);
      x(i) = 0;
      n = n+1;
  end
         
end

x(x==0) = [];
 y = y.';
z = vertcat(x,y);

w=hanning(1024);	%select a window
z=z.*w;		%window the signal
plot(z);
fftTransform=fft(z,44100);		%fourier transform, fft size = 44100
Mag=abs(fftTransform/22050);		%get the magnitude
Mag=Mag(1:44100/2); %omit redundant data from plot
plot(Mag);
grid;
%sound(x1,Fs);
%plot(x1(1:1024));

%plot(z);


