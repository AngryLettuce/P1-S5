function [ mel ] = freq2Mel( f )
%return mel from input frequency

mel = 1125*log(1 + f/700);

end

