function [ C ] = discreteCosineTransform( y )
% return the discrete cosine transform of y;
K = size(y,2);
C = zeros(1,K);

k = 0:K-1;

for n = 0:K-1
    C(n + 1) = sum(y.*cos(n.*(k + 0.5)*pi/K));
end

end

