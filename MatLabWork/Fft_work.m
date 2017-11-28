%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
 [x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\sine110Hz.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data

sampleBegin =1;
numOfSamples = 1024;
fftSize = 16384; % 2^14
x=x(sampleBegin:((numOfSamples-1)+sampleBegin)); % select sample chunk 
w=rectwin(numOfSamples);	%select a window
x=x.*w;		%window the signal

y=fft(x,fftSize);		%fourier transform, fft size = fftsize
y=circshift(y,-1);
Mag=abs(y/(numOfSamples/2));		%get the magnitude
Mag=Mag(1:(fftSize/2)); %omit redundant data from plot
plot(Mag);
grid;

sum = 0;
thres = max(Mag)*0.45;
maxPeak = 0;
n =1;

%loop through array and find peaks above specified threshold 
for i = 1:(fftSize/2)-1
    
    if Mag(i+1) >= thres
        
        if (Mag(i+1) > Mag(i)) &&(Mag(i+1) > Mag(i+2))
        maxPeak = Mag(i+1);
        peaks(n) = i+1;
        n = n+1;
             
        end
               
    end
    
end

%make new array containing freqency seperation between all adjacent peaks
for i = 1:(length(peaks))-1
    
    fDistance(i)  = (peaks(i+1)-peaks(i));
    
end

minfDistance = fDistance(1); %assume 1st element contains lowest value (aka lowest freq)

%scan array for lowest value
for i = 2:length(fDistance)
    
    if((fDistance(i) < minfDistance))
        minfDistance= fDistance(i);
    end
    
end

n=1;
%remove all octave errors
for i = 1:length(fDistance)
    
    if(fDistance(i) > minfDistance*1.15)
        
    else
        uniformFreqs(n) = fDistance(i);
        n=n+1;
        
    end
    
end

uniformFreqs %display results of formatted fft peak plot

%get average fundamental peak
for i = 1:(length(uniformFreqs))
    
    sum = uniformFreqs(i) + sum;
    
end


peaks

(sum/(length(uniformFreqs)))*2.69165 %
