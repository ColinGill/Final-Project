function rms = getRms(ArrOfSamples)


sum = 0; 

for i=1:length(ArrOfSamples)
    
   sum = sum +  ArrOfSamples(i)*ArrOfSamples(i);
   
end

rms = (sqrt(sum/length(ArrOfSamples)));
    

