function [ pitch ] = corrR2Pitch( R, sample_offset_min, sampling_rate )
%Extract pitch from R vector obtained from autocorrelation
%
%   sample_offset_min (sample)
%   sampling_rate (sample/sec) or (Hz), same things

sample_time = 1/sampling_rate;

[value sample_offset] = max(R(sample_offset_min:end));

pitch = floor(1/((sample_offset + sample_offset_min)*sample_time));


end

