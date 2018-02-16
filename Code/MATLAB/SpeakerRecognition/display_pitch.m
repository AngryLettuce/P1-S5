function [  ] = display_pitch( pitch )
%display pitch vector

pitch_mean = mean(pitch)*ones(size(pitch));
figure
hold on
plot(pitch)
plot(pitch_mean)
legend('tonalité dynamique',num2str(pitch_mean(1)))


end

