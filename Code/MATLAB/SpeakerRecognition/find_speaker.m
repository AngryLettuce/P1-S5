function [ speaker_ind ] = find_speaker( metric, codebook )

speaker_nb = size(codebook,2);


d_min = inf;
min_history_nb = 1;
i_min = zeros(1,min_history_nb);


% for i = 1:speaker_nb
%    
%     codeword_nb = size(codebook{i},1);
%     d_min(i) = 0;
%     
%     for j = 1:codeword_nb
%         
%         d_min(i) = d_min(i) + euclideanDistance(metric, codebook{i}(j,:));%find the power2 euclidien distance between test vector and codeword
%     end
% end
% 
% [V speaker_ind] = min(d_min);


for i = 1:speaker_nb
   
    codeword_nb = size(codebook{i},1);

    for j = 1:codeword_nb
        
        d_min_temp = euclideanDistance(metric, codebook{i}(j,:));%find the power2 euclidien distance between test vector and codeword

        if d_min_temp < d_min
            d_min = d_min_temp;%store the new minimum distance
            %j_min = j;%store the best fitted codeword index
            
            
            for k = 2:min_history_nb
               i_min(k) = i_min(k-1); 
            end
            i_min(1) = i;%store the best speaker
        end
    end
end

speaker_acc = zeros(1,speaker_nb);
for i = 1:min_history_nb
   speaker_acc(i_min(i)) = speaker_acc(i_min(i)) + 1;
end
[v speaker_ind] = max(speaker_acc);

end

