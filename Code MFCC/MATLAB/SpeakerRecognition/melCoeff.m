function [ mel_c ] = melCoeff( y, f1, f2, nb_filter, sample_rate, type)
%return mel coefficient from input signal 'y'

mel_c = (ones(nb_filter,1)*y) .* melFilterBank(f1, f2, nb_filter, size(y,2), sample_rate, type);
mel_c = sum(mel_c, 2)';

end

