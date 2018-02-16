function [ D ] = euclideanDistance( V1, V2 )
%find euclidienne distance between vector V1 and V2

D = sum((V1-V2).^2);

end

