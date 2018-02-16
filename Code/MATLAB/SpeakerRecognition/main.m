clear all
close all
clc

%add audio sample folder to project path
addpath('voice_sample_16bit_8kS');

config = 0;

[data,Fs] = audioread('Pascal_TS_1.wav');
sound(data,Fs)


data = data';
data = data(1,:);
data_mean = mean(data);
data_silence_threshold = mean(abs(data))/5;

figure(); plot(data)
title('inputed audio signal');


MelCoeff_nb = 13;
N = 256; %size of window
M = 100; %offset between each windows

y0 = 1;
n = 1;
Acc = zeros(1,MelCoeff_nb);


while(y0 + 255 < size(data,2))

    y = data(y0:y0+N-1);%get the proper 256 data block
    
    y0 = y0 + M;%update current data sample counter
    
    %check if average data is less than silence threshold
    %if the case skip all the process and continue to the next 256 block
    if (mean(abs(y)) <= data_silence_threshold) 
        continue;
    end
    
    %extract pitch from data block with autocorrelation
    R = autoCorrelation(y);
    pitch(n) = corrR2Pitch(R, 20, 8000);
    
%     figure()
%     plot(y) 
%     legend(num2str(pitch(n)))
    
    y = hammingWindow(y);%apply the hamming window to attenuate start and end of 256 block

    y = fft(y);%apply fft to 256 data block
    y = powerSpectral(y);%extract the power spectral from fft data
    y = y(1:floor(end/2));%discard second half of yFFT since it a duplicate of the first half

    y = melCoeff(y, 400, 8000, 20, 8000);%extract the melCoeff from the power spectrum
    y = log(y);%get the logarithm of the coeffcient
    
    %save data to variables
    C{n} = discreteCosineTransform(y(1:MelCoeff_nb));
    Acc = Acc + [pitch(n)/64 C{n}(2:end)];%increment the accumulator that average the metric for all the signal
    
    n = n + 1;%increment the metric vector number counter (n = numbre of metric vector)
end

Acc = Acc/n;%average the metric accumulator

%display pitch accross signal
pitch_mean = mean(pitch)*ones(size(pitch));
figure
hold on
plot(pitch)
plot(pitch_mean)
legend('tonalité dynamique',num2str(pitch_mean(1)))

C_dt = 1/(n-1);
C_t = 0:C_dt:1-C_dt;

[X, Y] = meshgrid(C_t, 0:size(C{1},2)-1);
Z = cell2mat(C')';




figure
surf(X, Y, Z,'EdgeColor','None');
view(2)


figure
hold on
plot(Z(5,:),Z(6,:),'o')
plot(Acc(5),Acc(6),'x')

%setting
if (config == 1)
    load('CB_v2')
    CB = [CB; Acc];
    save('CB_v2','CB')
end
%% test

load('CB_v2')

figure
hold on
plot(Acc(1:end),'o:')
for i = 1:size(CB,1)
   delta(i) = euclideanDistance(Acc(1:end), CB(i,1:end));
   plot(CB(i,1:end),'o-')
end
legend


figure
plot(delta);
[value min_index] = min(delta);

disp('Orateur détecté')
disp(min_index)










% 
% 
% 
% 
% 
% figure(); plot(y)
% title('inputed signal "y"');
% 
% 
% y = hammingWindow(y);
% figure(); plot(y)
% title('inputed signal "y" with hamming window');
% 
% y = fft(y);
% y = powerSpectral(y);
% %discard second half of yFFT since it a duplicate of the first half
% y = y(1:floor(end/2));
% figure(); plot(y)
% title('power spectrum');
% 
% 
% y = melCoeff(y, 100, 10000, 20, 10000);
% y = log(y);
% figure(); plot(y,'o')
% title('mel coefficient (dB)');
% 
% 
% 
