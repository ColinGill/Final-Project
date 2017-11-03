%Fft work

clear;

%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_F_88Hz_1sec.wav');	% get some data 
[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_E_82Hz_1sec.wav');	% get some data 
x=x(1:1024);		% select the sample frame, size =1024
w=hamming(1024);	%select a window
x=x.*w;		%window the signal
y=fft(x,44100);		%fourier transform, fft size = 44100
Mag=abs(y);		%get the magnitude
Mag=Mag(1:44100/2); %omit redundant data from plot
plot(Mag);
grid;

thres = max(Mag)*0.15
maxPeak = 0;
n =1;

%loop through array and find peaks above specified threshold 
for i = 1:(44100/2)-2;
    
    if Mag(i+1) >= thres
        
        if (Mag(i+1) > Mag(i)) &&(Mag(i+1) > Mag(i+2))
        maxPeak = Mag(i+1);
        peaks(n) = i+1;
        n = n+1;
             
        end
               
    end
    
end;

peaks
