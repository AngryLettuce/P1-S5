function [ metrics_gain_vector ] = extract_optimalMetricsGain( metrics, codebook, param )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

speaker_nb = size(metrics,2);


for i = 1:speaker_nb
    
    met_meanVec(i) = mean(metrics{i},1);
    met_varVec(i) = var(metrics{i},0,1);

    cb_meanVec(i) = mean(codebook{i},1);
    cb_varVec(i) = var(codebook{i},0,1);
    
end












end

