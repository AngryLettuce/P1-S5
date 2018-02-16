function [ audio_data ] = get_audioBank( folderName )
%construct audio bank data structure for fast MFCC algorithm analysis.

%the data is made of 3 level :
%
%
%  audio_data ---> speaker#1 ---> name
%              |              |
%              |-> speaker#2  |-> training_samples --------> sample#1
%             ...             |                          |
%              |-> speaker#n  |-> test_samples ->[...]   |-> sample#2
%                                                       ...
%                                                        |-> sample#n
%
%

audio_samples = dir(folderName);

sample_nb = size(audio_samples, 1) - 2; %number of sample in the inputed folder

speaker_ind = 1;%init speaker index to 1
training_ind = 1;%init training index to 1
test_ind = 1;%init test index to 1
last_name = ' ';%init last speaker name to '' (none)

audio_data = [];
training_samples = cell(0);
test_samples = cell(0);

for i = 1:1:sample_nb
    %get the name of the current sample
    name = audio_samples(i + 2).name;

    %split the name to get information about the nature of the sample
    name_splited = strsplit(name, '_');

    
    %check if the last name is different than curr one, if so, increment
    %speaker index, assign samples struct, and reset sublist index
    if ((~strcmp(last_name,name_splited{1}) && i ~= 1))
        audio_data(speaker_ind).name = last_name;
        audio_data(speaker_ind).training_samples = training_samples;
        audio_data(speaker_ind).test_samples = test_samples;
        
        training_samples = cell(0);
        test_samples = cell(0);
        
        speaker_ind = speaker_ind + 1;
        
        training_ind = 1;
        test_ind = 1;
    end  
    last_name = name_splited{1};
    
    
    
    %set sample name to proper structure field
    if (strcmp(name_splited{2},'CB')) 
        %sample is used to train the CodeBook (CB)
        training_samples{training_ind} = name;
        training_ind = training_ind + 1;
    else
        %sample used for test purpose (won't be use to train the system)
        test_samples{test_ind} = name;
        test_ind = test_ind + 1;     
    end
    
end

audio_data(speaker_ind).name = last_name;
audio_data(speaker_ind).training_samples = training_samples;
audio_data(speaker_ind).test_samples = test_samples;

training_samples = cell(0);
test_samples = cell(0);

speaker_ind = speaker_ind + 1;

training_ind = 1;
test_ind = 1;
    


end

