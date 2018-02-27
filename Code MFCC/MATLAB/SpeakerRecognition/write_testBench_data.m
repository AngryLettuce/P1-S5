function [  ] = write_testBench_data( filename, x, y )


N = size(x,2);

for i = 1:N
    filename_x = ['testBench/',filename,'_x',num2str(i),'.csv'];
    filename_y = ['testBench/',filename,'_y',num2str(i),'.csv'];
    
    %write header to file
    fid = fopen(filename_x,'w'); 
    fprintf(fid,'%s\n',[num2str(size(x{i},1)), ',', num2str(size(x{i},2))]);
    fclose(fid);

    fid = fopen(filename_y,'w'); 
    fprintf(fid,'%s\n',[num2str(size(y{i},1)), ',', num2str(size(y{i},2))]);
    fclose(fid);

    dlmwrite(filename_x,x{i},'-append');   
    dlmwrite(filename_y,y{i},'-append');
end



end

