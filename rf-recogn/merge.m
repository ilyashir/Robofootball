%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                              %%%%
%%%% ������� ��� ��������� �������� ������������� %%%%
%%%% ��������� �� ������ ������������ ���         %%%%
%%%%                                              %%%%
%%%% @author: "RoboFootball SPbSU team"           %%%%
%%%% @email: robofootball@gmail.com               %%%%
%%%%                                              %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [equiv_clusters,diffs]=merge(list)

equiv_clusters=[];
count=0;

% ���� � ������ ��� ���� ��������
while (size(list,2)>0)
    
    % ��������� ��������� ��� �������� �������� 
    % ������ ������������� ���������
    tmp=[list{1}(1)];
    tmp_length=1;
    j=1;
    
    % ��� ������� �������� � ������
    while(j<=length(tmp))
        
        % ������� �������
        start=tmp(j);
        i=1;
        
        %������������� �� ����� ������ ���
        while (i<=size(list,2))
            
            % ���� � ���� ����� - ������� �������,
            % �� ��������� ������� ������ � ������� ������
            % ������������� ���������.
            % � ���� �������
            if (list{i}(1)==start)
                if(check_1dim(tmp,list{i}(2))==0)
                    tmp_length=tmp_length+1;
                    tmp(tmp_length)=list{i}(2);
                end
                list(i)=[];
                i=i-1;
            else
                % ����������, ���� ������� - � ���� ������
                if (list{i}(2)==start)
                    if(check_1dim(tmp,list{i}(1))==0)
                        tmp_length=tmp_length+1;
                        tmp(tmp_length)=list{i}(1);
                    end
                    list(i)=[];
                    i=i-1;
                end
            end
        i=i+1;
        end
    j=j+1;
    end
    count=count+1;
    
    % ��������� ������ ��������� ������������� 
    % ��������� � ����� ���������
    equiv_clusters{count}=tmp;
end

% � ������ ������ ����� ������ ������� ���������,
% � ������� ���� ������������� (����� �����
% �������� "��������"

diffs=[];
count=0;
for i=1:length(equiv_clusters)
    for j=1:size(equiv_clusters{i},2)
        count=count+1;
        diffs(count)=equiv_clusters{i}(j);
    end
end
