function [ M ] = extract_speakerMetrics(data, metrics, param )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here

global DEBUG


data_size = size(data,2) - 1;

%pre-emphasis
for i = 2:data_size + 1
   data(i-1) = data(i-1) - 0.95*data(i);  
end


N = param.N;
M = param.M;

%extract time info
sample_time = data_size / param.sample_rate;    %time for a single sample
window_time = N * sample_time;                  %time for a whole window (N sample)
between_window_time = M * sample_time;          %time between each window frame (M sample)

data_mean = mean(data);
data_silence_threshold = 0.005;

curr_t = 0;%init surr time to zero
y0 = 1; %start index of the current data block (M sample)
n = 1;  %number of metrics vector (increase as the algorithm pass through the signal)
metrics_acc = zeros(1,param.mfccCoeff_nb + 1);%metric accumulator


% if DEBUG == 1
%     
%     for i = 1:size(data,2)-256
%         yt(i) = mean(abs(data(i:i+256)));
%     end
%     
%     figure()
%     hold on
%     plot(data./max(data))
%     plot(yt./max(yt))
% 
%     
%     figure
%     hold on
%     plot(data)
%     plot(ones(size(data)).*data_silence_threshold)
%     
%     disp(' ')
% end



while(y0 + N - 1 < size(data,2))

    y = data(y0 : y0 + N - 1);%get the proper 256 data block
    
    y0 = y0 + M;%update current data sample counter
    
    %update time
    curr_t = curr_t + between_window_time;
    
    %check if average data is less than silence threshold
    %if the case skip all the process and continue to the next 256 block
    if (mean(abs(y)) <= data_silence_threshold) 
        continue;
    end
      
    C{n} = mfcc_extraction(y, param);
    C{n} = C{n}(2:param.mfccCoeff_nb + 1);%we remove the first coefficient because it's speaker independente, it only represent the average volume of the speech
   
    pitch(n) = pitch_extraction(y, param);
    pitch_norm(n) = (pitch(n) - param.pitch_norm_offset) * param.pitch_norm_gain;%normalize the pitch to be on same scale as MFCC coefficient for euclidien distance evaluation
    
    %metrics_acc = metrics_acc + [pitch(n)/64 C{n}];%increment the accumulator that average the metric for all the signal
    
    t(n) = curr_t;  %assign the proper time with the current metrics vector
    n = n + 1;      %increment the metric vector number counter (n = numbre of metric vector)

end


%metrics_acc = metrics_acc/n;%average the metric accumulator
%display_pitch(pitch);       %display pitch accross signal

C = cell2mat(C')';%convert MFCC coeff cell array to matrix

if DEBUG == 1
    figure
    surf(C,'EdgeColor','None')
    view(2)
end


%update the metrics matrix at "index"'ith speaker 
normalization = [1 1 linspace(1,1,10)];
M = [metrics ; pitch_norm' C'.*normalization];

end

