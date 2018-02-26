clear all
close all
clc

global DEBUG
DEBUG = 0;

c = strsplit(num2str(fix(clock)));%get time and date information

%%%%%%%%%%%%%%%%%%
% PARAM INIT
%%%%%%%%%%%%%%%%%%

%file and codebook parameters
param.codeBook_name = ['codeBook_' c{1} '-' c{2} '-' c{3} '_' c{4} 'h' c{5} 'm' c{6} 's.mat'];%generate a name with time and date for archive
param.sample_folder = 'voice_sample_16bit_8kS_v2';%folder containing all audio sample

%codebook parameters
param.codebook_M = 16;
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
param.index_avg = 100;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% METRICS TRAINING SAMPLE EXTRACTION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp(' ')
disp('--------------------------------------')
disp('Executing training sequence')
disp('--------------------------------------')
tic

%add audio sample folder to project path
addpath(param.sample_folder);
%get audio data structure
audio_data = get_audioBank(param.sample_folder);

speaker_nb = size(audio_data,2);
metrics = cell(1,speaker_nb);%cell variable containing all metric vectors of each speakers extracted from audio sample
%
%metrics{1}  = window1 [T C1 C2 C3 ... Cn]      (each index of metrics is a nb_Of_Metrics by nb_Window_Frame matrix)
%              window2 [T C1 C2 C3 ... Cn]
%                ...
%              windown [T C1 C2 C3 ... Cn]

%extract all metric vector for each speaker and store those vectors inside cell var. "metrics" 
for i = 1:speaker_nb
    tstart = tic;
    
    training_sample_nb = size(audio_data(i).training_samples, 2);
    for j = 1:training_sample_nb
        
        %load audio file of current training set
        [data,Fs] = audioread(audio_data(i).training_samples{j});
        data = data(:,1)';%reformat data
        metrics{i} = extract_speakerMetrics(data, metrics{i}, param);  
    end
    %display time
    disp(' ')
    disp(['Speaker ', num2str(i),' metrics extracted' ])
    
    toc(tstart)
end

disp(' ')
disp('//Total time :')
toc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CODE BOOK CONSTRUCTION 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tic
disp(' ')
disp('--------------------------------------')
disp('Executing codebook construction')
disp('--------------------------------------')
disp(' ')
%construct the codebook for each speakers
codebook = construct_speakerCodeBook(metrics, param);

disp(' ')
disp('//Total time :')
toc

 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % METRICS OPTIMAL GAIN VECTOR
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% tic
% disp(' ')
% disp('--------------------------------------')
% disp('Executing optimal gain vector extraction')
% disp('--------------------------------------')
% disp(' ')
% %construct the codebook for each speakers
% metrics_gain_vector = extract_optimalMetricsGain(metrics, codebook, param);
% 
% disp(' ')
% disp('//Total time :')
% toc



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% TEST WITH UNSEEN SAMPLE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp(' ')
disp('--------------------------------------')
disp('Executing test samples metrics  extraction')
disp('--------------------------------------')
tic

%extract all metric vector for each speaker and store those vectors inside cell var. "metrics" 
for i = 1:speaker_nb
    tstart = tic;
    
    test_sample_nb = size(audio_data(i).test_samples, 2);
    for j = 1:test_sample_nb
        
        %load audio file of current training set
        [data,Fs] = audioread(audio_data(i).test_samples{j});
        data = data(:,1)';%reformat data
        metric = extract_speakerMetrics(data, [], param);  
        
        
        %TEST -----
        
        %dynamic check test (one data block at a time)
        if param.test_dynamic_check == 1
            length = size(metric,1);
            speaker_ind = zeros(1, length);

            for k = 1:length
                speaker_ind(k) = find_speaker(metric(k), codebook);
            end

            figure()
            plot(speaker_ind,'o-');
            title(audio_data(i).name)
        end
        
        %dynamic check test (moving average)
        if param.test_dynamic_movAvg_check == 1
            length = size(metric,1);
            metric_nb = floor(param.dynamic_check_period / param.metric_vector_period);
            speaker_ind = zeros(1, length - metric_nb);

            for k = 1:length - metric_nb
                metric_mean = mean(metric(k:k + metric_nb,:), 1);
                speaker_ind(k) = find_speaker(metric_mean, codebook);
            end

            
            %averaging index
            length_adjusted = length - metric_nb - param.index_avg;
            for k = 1:length_adjusted
                speaker_ind(k) = mode(speaker_ind(k:k + param.index_avg));
            end
            
            %accuracy
            stat(i,j) = sum(speaker_ind(1:length_adjusted) == i)/(length - metric_nb - param.index_avg);
            disp(stat(i,j))
            
            figure()
            plot(speaker_ind(1:length_adjusted),'o-');
            title(audio_data(i).name)
            
           
            
        end
        
        %one shot time check
        if param.test_singleShot_check == 1
            speaker_ind = find_speaker(mean(metric), codebook)
        end
        
        %TEST END -----
        
    end
    %display time
    disp(' ')
    disp(['Speaker ', num2str(i),' metrics extracted' ])
    
    toc(tstart)
end

toc




