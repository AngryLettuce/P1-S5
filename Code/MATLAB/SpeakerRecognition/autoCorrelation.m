function [ R ] = autoCorrelation( X )
%create the vector R 


N = size(X,2);
K = floor(N/2);

R = zeros(1,K);

for k = 1:K
    n = 1:N-k;
    R(k) = mean(X(n).*X(n+k));
end

end



