%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                %%%%
%%%% ������� ��� ���������� ������� ������,         %%%%
%%%% � ����� ��� ������������ ���������,            %%%%
%%%% � ������� �������� ������� ��� ������� �����   %%%%
%%%%                                                %%%%
%%%% @author: "RoboFootball SPbSU team"             %%%%
%%%% @email: robofootball@gmail.com                 %%%%
%%%%                                                %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [avg,list_color,npoint]=find_averages(clust,N_elem,pic,N_color)
avg=[];

for k=1:4
    st=1;
    col_size = 0;
    
    sumR = 0;
    sumG = 0;
    sumB = 0;
    
    for i=1:size(N_color{k},2)  
        numcl = N_color{k}(i);
        col_size = col_size + N_elem(numcl);
        
        for j=1:N_elem(numcl);
            
            % ���������� �������
            x=clust{numcl}(1,j);
            y=clust{numcl}(2,j);
            
            % ����� �� �����������
            sumR=sumR+pic(x,y,1);
            sumG=sumG+pic(x,y,2);
            sumB=sumB+pic(x,y,3);
            
            % ��������� ���������� ������� � ������ 
            % ��� ������� �����
            list_color{k}(st,:)=[pic(x,y,1) pic(x,y,2) pic(x,y,3)];
            st = st+1;
        end
    end
    npoint(k) = st-1;
    
    % ������ �������� �����
    avg{k} = [sumR, sumG, sumB]/col_size;
end