function [ codebook ] = construct_speakerCodeBook( metrics, param )
%construct_speakerCodeBook construct the code book for each speaker from
%metrics vector extracted from audio sample

global DEBUG






%convergence parameters
M           = param.codebook_M;
esp         = param.codebook_eps;
dist_esp    = param.codebook_dist_esp;
iter_limit  = param.codebook_iter_limit;


speaker_nb = size(metrics,2);

%init cell, vector and matrix
codebook = cell(1,speaker_nb);
C_acc = zeros(M,param.metrics_nb);
C_vector_nb = zeros(M,1);

%create a fig that will display codebook of all speaker stacked on the same figure
if DEBUG == 1
    fig_all_MFCC_56 = figure();
    fig_all_MFCC = figure();
end

for i = 1:speaker_nb
    
%     %create speaker dependant fig for debug
%     if DEBUG == 1
%         fig_conv = figure();
%         fig_MFCC_56 = figure();
%     end

    
    vector_nb = size(metrics{i},1);%get the number of metric vectors for a specific speaker

    C = mean(metrics{i},1);%get the first codeword, which is the centroid of all training vectors the specific speaker
    m = 1; %number of codebook's codeword
    
    
    while(m < M)
    
        %init distorion convergence value
        dist_conv = inf;
        
        %split each codeword centroid in 2
        Cp = C .* (1 + esp);%positive split
        Cn = C .* (1 - esp);%negative split
        C = [Cp; Cn];%extend codebook with the new codewords
        
        %update codeword number
        m = 2*m;
        
        %reset centroid accumulator 
        C_acc(1:m,:) = zeros(m,param.metrics_nb);
        %reset codeword number of training vector buffer
        C_vector_nb(1:m) = 0;
        %reset iter counter
        iter = 0;
        D_last = inf;
        
        while(dist_conv > dist_esp && iter < iter_limit)    

            for j = 1:vector_nb
                
                C_min = inf;
                for k = 1:m
                    
                    C_min_temp = euclideanDistance(metrics{i}(j,:), C(k,:));%find the power2 euclidien distance between training vector and codeword
                    
                    if C_min_temp < C_min
                        C_min = C_min_temp;%store the new minimum distance
                        k_min = k;%store the best fitted codeword index
                    end
                end 
                
                C_acc(k_min,:) = C_acc(k_min,:) + metrics{i}(j,:); 
                C_vector_nb(k_min) = C_vector_nb(k_min) + 1;
                
            end
            
            %update the codebook with new centroid got with matched training vectors
            C = C_acc(1:m,:) ./ (C_vector_nb(1:m)*ones(1,param.metrics_nb));

            %get the distortion between traning vectors (metrics) and codebook
            D = distortion(metrics{i}, C);
            
            dist_conv = (D_last - D)/D;
            
            D_last = D;
            
            iter = iter + 1;
            
            dist_conv_hist(iter) = dist_conv;
        end
        
%         %for debug purpose
%         if DEBUG == 1
%             figure(fig_conv)
%             hold on
%             plot(1:iter,dist_conv_hist(1:iter),'o-');
%             legend()
%             title('Centroid ajustement distortion convergence')
%             xlabel('Iteration')
%             ylabel('Distortion convergence rate')
%         end
        
        
    end
    
    codebook{i} = C;
    
%     %for debug purpose
%     if DEBUG == 1
%         figure(fig_MFCC_56)
%         hold on
%         plot(metrics{i}(:,6), metrics{i}(:,7),'o','MarkerSize',1)
%         plot(C(:,6),C(:,7),'o','linewidth',2,'MarkerSize',5)
%         title('MFCC, C_6 and C_7 distribution')
%         xlabel('MFCC coefficient : C_6')
%         ylabel('MFCC coefficient : C_7')
%         legend('training vectors',['Codebook codewords, M = ', num2str(m)])
%     end
    
    %for debug purpose
%     if DEBUG == 1
%         figure(fig_all_MFCC_56)
%         hold on
%         plot(metrics{i}(:,6), metrics{i}(:,7),'o','MarkerSize',1)
%         plot(C(:,6),C(:,7),'o','linewidth',2,'MarkerSize',5)
%         title('MFCC, C_6 and C_7 distribution')
%         xlabel('MFCC coefficient : C_6')
%         ylabel('MFCC coefficient : C_7')
%         legend('training vectors',['Codebook codewords, M = ', num2str(m)])
% 
%         figure()%fig_all_MFCC)
%         hold on
%         for k = 1:m
%             plot(codebook{i}(k,:),'-o','color',[0.5 i/speaker_nb 1-i/speaker_nb])
%         end
% 
%     end

    
end


end

