function [ w ] = hammingWindow( y )
%return 'w' the inputed signal 'y' multiplied to the hamming window 

%this function fade out the begining and the ending of the signal 'y' in order to reduce
%spectral leakage after aplying FFT

N = size(y,2);
n = 0:N-1;

HW = 0.54 - 0.46*cos(2*pi*n/(N-1));

w = y.*HW; 

end

