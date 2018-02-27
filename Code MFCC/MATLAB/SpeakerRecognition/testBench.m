clear vars
close all
clc

%% MODULE FFT
%hammingWindows --------------------------------
x{1} = ones(1,256);
x{2} = -5*ones(1,256);
x{3} = (-1).^ones(1,256);
for i = 3:-1:1
    y{i} = hammingWindow(x{i});
end
write_testBench_data('mfcc_hammingWindow',x,y);


%fft --------------------------------------------
x{1} = load('audio_ex1.mat','y'); x{1} = x{1}.y;
x{2} = load('audio_ex2.mat','y'); x{2} = x{2}.y;
x{3} = load('audio_ex3.mat','y'); x{3} = x{3}.y;
for i = 3:-1:1
    y{i} = fft(x{i});
    y_complex{i} = [real(y{i}),imag(y{i})];
end
write_testBench_data('mfcc_fft',x,y_complex);


%powerSpectrum ----------------------------------
x{1} = y{1};
x{2} = y{2};
x{3} = y{3};
for i = 3:-1:1
    powerSpectrum{i} = abs(x{i}).^2;
    y{i} = [real(powerSpectrum{i}),imag(powerSpectrum{i})];
end
write_testBench_data('mfcc_powerSpectrum',x,y);

%% MODULE MEL FILTER BANK
%freq2Mel ---------------------------------------
x{1} = (1:1000);
y{1} = freq2Mel(x{1});
write_testBench_data('freq2Mel',x,y);

%mel2Freq ---------------------------------------
x{1} = (1:1000);
y{1} = mel2Freq(x{1});
write_testBench_data('mel2Freq',x,y);


%melFilterBank ----------------------------------
x{1} = [10 , 4000, 20, 256, 8000];
x{2} = [0  , 4000, 24, 256, 8000];
x{3} = [500, 3000, 20, 256, 8000];
for i = 3:-1:1
    melFilters{i} = melFilterBank(x{i}(1) , x{i}(2), x{i}(3), x{i}(4), x{i}(5), true);
end
write_testBench_data('mfcc_melFilterBank',x,melFilters);


%powerSpectrum X melFilterBank ------------------
for i = 3:-1:1
    y{i} = sum(melFilters{i} .* (ones(size(melFilters{i}, 1),1) * powerSpectrum{i}),2)';
end
write_testBench_data('mfcc_coeff',x,melFilters);




