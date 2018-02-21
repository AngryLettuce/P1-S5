function [ pitch ] = pitch_extraction( y, param )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

    %extract pitch from data block with autocorrelation
    R = autoCorrelation(y);
    pitch = corrR2Pitch(R, 20, param.sample_rate);
    
    
end

