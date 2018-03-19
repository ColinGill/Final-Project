%Fft work - Guitar pitch detection - 

clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
%[x ,Fs]=audioread('C:\\Users\\Donnacha\\Desktop\\Final-Project\\chunks\\1024chunk_3_.wav');
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\E2powerChord.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_F_88Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\E_F_F#_G.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\noiseTestLowFreqs.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\C_B_A_G_F#_E_fast.wav');	% get some data 
%[x ,Fs]=audioread('C:\\Users\Donnacha\Desktop\\Final-Project\\GuitarNotes+TestSignals\\noiseTestE82Hz.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\G_F#_F_E.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\A2powerChord.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_E_82Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Low_A_110Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Octave_D_292Hz_1sec.wav');	% get some data
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\FastGuitar.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\constantAnote.wav');	% get some data 
 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\constantAnoteMuted.wav');
  [x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\Chromatic_Low_notes.wav');
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\noiseTestHighFreqs.wav');
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\High_F_695Hz_1sec.wav');	% get some data
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\C_B_A_G_F#_E_fast.wav');

sampleBegin =1;
numOfSamples = 512;
numOfchunks = 512;

lastChunk = length(x)/numOfSamples;
lastChunk = floor(lastChunk);
subplot(3,1,1);
plot(x);
%sound(x,Fs);
belowThreshold = 0;
start = 1;
ending = numOfchunks;
chnk = 1;
oldFrequency =0;
ignoreChunk = 0;
cnt = 1;
count = 1;
previousCnkValue = 1;
RecordNote =0;

for i=1:lastChunk
    
    x2=x(sampleBegin:((numOfSamples-1)+sampleBegin)); % select sample chunk 
          
    [s, numOfchunks] = sumabs(x2);
    chunkValue(chnk) =  s/numOfSamples;
    rmsArr(chnk) = getRms(x2);        
    chnk = chnk + 1; 
    sampleBegin = numOfSamples + sampleBegin;

end

subplot(3,1,2);
stairs(chunkValue);

subplot(3,1,3);
stairs(rmsArr);

 