function [ f ] = mel2Freq( mel )
%return frequency from inputed mel

f = 700*exp(mel/1125) - 700;

end

