
clear;

n = 1:1024;
Fs = 1024;

y = sin(2*pi*n*9/Fs)+ 0.5*sin(2*pi*n*80/Fs) + 0.3*sin(2*pi*n*300/Fs); %generate composite waveform

%correlate with 1-512hz sine waves
for t = 1:512
    
    sum =0;
    x = sin(2*pi*n*(t/Fs)); %increment 1 hz each cycle
    
    for i = [1:1:1024]

        sum = sum + x(i) * y(i);
      
    end
    
    mydft(t) = sum;
    sum =0;
end

subplot(2,1,1);
plot(y);
title('input');
xlabel('samples');
ylabel('Amplitude');

subplot(2,1,2);
plot(mydft);
title('dft');
xlabel('Hz');
ylabel('Magnitude');
