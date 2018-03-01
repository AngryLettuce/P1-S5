function [moving_average] = movingAverage(samples, acc_size, step_size)


moving_average = zeros(length(samples)/step_size, 1);
j = 1;
for i = 1 : step_size : length(samples)
   
    endingIndex = i + acc_size; 
    
    if endingIndex > length(samples)
        endingIndex = length(samples);
    end
        
   moving_average(j) = mean(samples(i : endingIndex));
   j = j + 1;
    
end



