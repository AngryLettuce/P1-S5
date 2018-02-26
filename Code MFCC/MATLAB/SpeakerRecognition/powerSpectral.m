function [ p ] = powerSpectral( fft_k )
%return the power of frequency

N = size(fft_k,2);
p = abs(fft_k).^2;

end

