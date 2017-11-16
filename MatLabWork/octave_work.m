clear;

%AUDIO TEST FILES (Uncomment for use in script.. will throw error unless one is uncommented)
%[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_F_88Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_E_82Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Low_A_110Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\Octave_D_292Hz_1sec.wav');	% get some data 
%[x ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\GuitarNotes\High_F_695Hz_1sec.wav');	% get some data 
[x1 ,Fs]=audioread('C:\Users\Donnacha\Desktop\Y5 Project\Asinewave.wav');	% get some data 

x=x1(1:22050);		% select the sample frame, size =881 (20 ms)
n=1;

for i=1:22050;
   
    remainder = mod(i,2);
  if (remainder == 1)
      y(n)=x(i);
      x(i) = 0;
      n = n+1;
  end
  
        
end
x(x==0) = [];
 x = x.';
z = horzcat(x,y)

%sound(x1,Fs);
%plot(x1(1:1024));
sound(z,Fs);
plot(z);
%plot(z);


