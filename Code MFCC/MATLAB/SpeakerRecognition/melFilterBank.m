function [ melFB ] = melFilterBank( f1, f2, nb, size_data, sample_rate, triangle)
%return array of size 'nb' of mel filter of 'size_data' points covering frequency from 'f1' to 'f2' 

N = nb + 2;

mel(N) = freq2Mel(f2);%start with end to save CPU time
mel(1) = freq2Mel(f1);

delta_mel = (mel(N) - mel(1))/(N - 1);

mel(:) = mel(1):delta_mel:mel(N);


f = mel2Freq(mel);
f = floor((size_data)*f/(sample_rate/2));

melFB = zeros(nb, size_data);%preassign mel filter bank matrix

n = 0:size_data-1;
for i = 2:nb+1
    C1 = n >= f(i-1) & n <= f(i);
    C2 = n > f(i) & n <= f(i+1);
    
    if triangle == true
        melFB(i-1, C1) = (n(C1) - f(i-1))/(f(i) - f(i-1));
        melFB(i-1, C2) = (f(i+1) - n(C2))/(f(i+1) - f(i));
    else
        melFB(i-1, C1) = 1;
        melFB(i-1, C2) = 1;
    end
end

% figure
% for i = 1:size(melFB,1)
%     hold on
%     plot(melFB(i,:),'o-')
% end 
% 
% disp(' ')

end

