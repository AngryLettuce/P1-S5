function [ D ] = distortion( metric, C )

D = 0;
vector_nb = size(metric,1);
m = size(C,1);

for j = 1:vector_nb

    C_min = inf;
    for k = 1:m

        C_min_temp = euclideanDistance(metric(j,:), C(k,:));%find the power2 euclidien distance between training vector and codeword

        if C_min_temp < C_min
            C_min = C_min_temp;%store the new minimum distance
            C_min_ind = k;%store the best fitted codeword index
        end
    end 

    D = D + C_min;

end

D = 1/vector_nb * D;%get the mean of minimal distance between training vector and codeword

end

