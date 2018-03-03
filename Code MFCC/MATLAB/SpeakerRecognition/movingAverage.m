function [moving_average] = movingAverage(samples, acc_size, step_size)


moving_average = zeros(length(samples)/step_size, 1);
j = 1;
beta_acc = 0;
alpha =  0;
alpha_p = 0;

for i = 1 : step_size : length(samples)
   
%     endingIndex = i + acc_size -1;
% 
%     if endingIndex > length(samples)
%         endingIndex = length(samples);
%     end
%     
     beta_acc = sum(samples(i:i+step_size -1));   
     moving_average(j) = (alpha_p * (acc_size - 2 * step_size)/step_size + alpha + beta_acc)/acc_size;  
     
%    moving_average(j) = mean(samples(i : endingIndex));
   j = j + 1;
   alpha_p = alpha;
   alpha   = beta_acc;
   beta_acc = 0;
    
end



