clear vars
close all
clc

%% MODULE FFT
%hammingWindows --------------------------------
x{1} = ones(1,256)';
x{2} = -5*ones(1,256)';
x{3} = (-1).^ones(1,256)';
for i = 3:-1:1
    y{i} = hammingWindow(x{i}')';
end
write_testBench_data('mfcc_hammingWindow',x,y,3);


%fft --------------------------------------------
x{1} = load('audio_ex1.mat','y'); x{1} = x{1}.y';
x{2} = load('audio_ex2.mat','y'); x{2} = x{2}.y';
x{3} = load('audio_ex3.mat','y'); x{3} = x{3}.y';
for i = 3:-1:1
    y{i} = fft(x{i});
    y_complex{i} = [real(y{i}),imag(y{i})];
end
write_testBench_data('mfcc_fft',x,y_complex,3);


%powerSpectrum ----------------------------------
x{1} = y{1};
x{2} = y{2};
x{3} = y{3};
for i = 3:-1:1
    powerSpectrum{i} = abs(x{i}).^2;
    y{i} = [real(powerSpectrum{i}),imag(powerSpectrum{i})];
end
figure
plot(y{1})

write_testBench_data('mfcc_powerSpectrum',x,y,3);

%% MODULE MEL FILTER BANK
%freq2Mel ---------------------------------------
x{1} = (1:5:1000)';
y{1} = freq2Mel(x{1});
write_testBench_data('freq2Mel',x,y,1);

%mel2Freq ---------------------------------------
x{1} = (1:5:1000)';
y{1} = mel2Freq(x{1});
write_testBench_data('mel2Freq',x,y,1);


%melFilterBank ----------------------------------
x{1} = [10 , 4000, 20, 256, 8000];
x{2} = [0  , 4000, 24, 256, 8000];
x{3} = [500, 3000, 20, 256, 8000];
for i = 3:-1:1
    melFilters{i} = melFilterBank(x{i}(1) , x{i}(2), x{i}(3), x{i}(4), x{i}(5), true);
end
write_testBench_data('mfcc_melFilterBank',x,melFilters',3);


%% Moving Average

t = 1:2500;
x{1} = abs(sin(3*t) + sin(4*t));
acc_size  = 256;
step_size = 100;

moving_average{1} = movingAverage(x{1}, acc_size, step_size);
write_testBench_data('moving_average',x,moving_average);





%powerSpectrum X melFilterBank ------------------
for i = 3:-1:1
    y{i} = sum(melFilters{i} .* (powerSpectrum{i} * ones(1,size(melFilters{i},2))),1)';
end
write_testBench_data('mfcc_coeff',x,melFilters);





