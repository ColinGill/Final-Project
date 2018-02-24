 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\NormalizeTest.wav');
 %[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\FastGuitar.wav');
 [x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Final-Project\GuitarNotes+TestSignals\C_B_A_G_F#_E_fast.wav');
 
sampleBegin =1;
numOfSamples = 1025;
numOfchunks = 256;
 
lastChunk = floor(length(x)/numOfSamples); 
subplot(2,1,1);
plot(x);
chunkNum =1; 
sPosition =1;
sEnd = 1025;
nn = 1;
 
 for i=1:lastChunk
     
     
      x2=x(sampleBegin:((numOfSamples-1)+sampleBegin));
      maxSample = max(abs(x2));
      multiplier = 1/maxSample;
      
      for n= sPosition:sEnd
          
          normalizedSignal(n) = x2(nn)*multiplier;
          nn = nn +1;
      
      end
      
      nn=1;
      sPosition = sPosition+1024;
      sEnd  = sEnd + 1024;
      sampleBegin = sampleBegin + numOfSamples;
 end
 
  subplot(2,1,2);
  plot(normalizedSignal);
 