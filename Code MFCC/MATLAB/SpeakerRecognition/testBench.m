clear vars
close all
clc

%% PARAMETERS INIT

%file and codebook parameters
param.codeBook_name = ['codeBook_' c{1} '-' c{2} '-' c{3} '_' c{4} 'h' c{5} 'm' c{6} 's.mat'];%generate a name with time and date for archive
param.sample_folder = 'voice_sample_16bit_8kS_v2';%folder containing all audio sample

%codebook parameters
param.codebook_M = 32;
param.codebook_eps = 0.001;
param.codebook_dist_esp = 0.001;
param.codebook_iter_limit = 100;

%mfcc parameters
param.N = 256; %size of window
param.M = 100; %offset between each windows
param.mfccCoeff_nb = 12;

%total metrics vector dimension (number of term)
param.metrics_nb = param.mfccCoeff_nb + 1;


param.melFilter_nb = 20;%nombre of Mel filter in the filter bank
param.melFilter_flc = 40;%lower bound frequency for Mel filter bank
param.melFilter_flh = 4000;%higher bound frequency for Mel filter bank
param.melFilter_triangular = true;%if true, Mel filter bank consist of triangle filter, if false, Mel filter bank consist of rectangle filter
param.sample_rate = 8000;%sample rate of the ADC, in this case, sample rate of inputed .wav files

%pitch parameters
param.pitch_norm_offset = 150;
param.pitch_norm_gain = 1/8;

%test analysis parameters
param.dynamic_check_period = 0.0125; %period in second
param.metric_vector_period = param.M / param.sample_rate; %period of time attribuated to a metric vector extracted from sound data

param.test_dynamic_check        = 0;
param.test_dynamic_movAvg_check = 1;
param.test_singleShot_check     = 0;

%index parameters
param.index_avg = 0;


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
x{1} = [10 , 4000, 20, 128, 8000];
x{2} = [0  , 4000, 24, 128, 8000];
x{3} = [500, 3000, 20, 128, 8000];
for i = 3:-1:1
    melFilters{i} = melFilterBank(x{i}(1) , x{i}(2), x{i}(3), x{i}(4), x{i}(5), true);
end
write_testBench_data('mfcc_melFilterBank',x,melFilters',3);


%get Mel coeff -----------------------------------
x{1} = powerSpectrum{1}(1:end/2);
x{2} = powerSpectrum{2}(1:end/2);
x{3} = powerSpectrum{3}(1:end/2);

for i = 3:-1:1
    melCoeff{i} = log10(sum(ones(size(melFilters{i},1),1) * x{i}' .*  melFilters{i},2));
end
write_testBench_data('mfcc_getMelCoeff',x,melCoeff,3);


%% Discret Cosinus Transform
x{1} = melCoeff{1};
x{2} = melCoeff{2};
x{3} = melCoeff{3};

for i = 3:-1:1
	y{i} = discreteCosineTransform(x{i}')';
end
write_testBench_data('mfcc_dct',x,y,3);



%% Moving Average

t = 1:2500;
x{1} = abs(sin(3*t) + sin(4*t));
acc_size  = 256;
step_size = 100;

moving_average{1} = movingAverage(x{1}, acc_size, step_size);
write_testBench_data('moving_average',x,moving_average',1);



%% MFCC PIPELINE


x{1} = load('audio_ex1.mat','y'); x{1} = x{1}.y';
x{2} = load('audio_ex2.mat','y'); x{2} = x{2}.y';
x{3} = load('audio_ex3.mat','y'); x{3} = x{3}.y';
for i = 3:-1:1
    y{i} = mfcc_extraction(x{i}', param)';
end
write_testBench_data('mfcc_pipeline',x,y,3)













