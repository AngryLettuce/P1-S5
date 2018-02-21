function [ C ] = mfcc_extraction( y, param )
%metrics extraction script

    global DEBUG 

    y = hammingWindow(y);%apply the hamming window to attenuate start and end of 256 block

    y = fft(y);%apply fft to 256 data block
    y = powerSpectral(y);%extract the power spectral from fft data
    y = y(1:floor(end/2));%discard second half of yFFT since it a duplicate of the first half

%     if DEBUG == 1
%        figure(1)
%        plot(y,'o-')
%     end
    
    y = melCoeff(y, param.melFilter_flc, param.melFilter_flh, param.melFilter_nb, param.sample_rate, param.melFilter_triangular);%extract the melCoeff from the power spectrum
    
    
    y = log(y);%get the logarithm of the coeffcient
    
    %save data to variables
    C = discreteCosineTransform(y);
end

