%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                       %%%%
%%%% ������� ��� ��������� ���������       %%%%
%%%%                                       %%%%
%%%% @author: "RoboFootball SPbSU team"    %%%%
%%%% @email: robofootball@gmail.com        %%%%
%%%%                                       %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [clust,N_elem,N_clust, img,diffs] = find_clusters(pict4, handles)

j_min = 1;
i_min = 1;
[i_max j_max] = size(pict4);

% ���������� ���������
N_clust = 0;
% ������ ���������� ��������� � ������ ��������
N_elem=[];

% ��� �������� ��� ������������� ���������
equiv=[];
equiv_count=0;

% ����� �������� �������� ��� ���������� ������
string_prev = zeros(1,j_max);

% ���� ��������
clust=[];

% �������� ������� �������� � ������ ������ (���� �� ��� ����)
if (pict4(i_min,j_min) > 0),
    N_clust = 1;
    string_prev(j_min) = N_clust;
    N_elem(N_clust) = 1;
    clust{1}(1:2,1) = [1; 1];
end

% �������� ��������� ��������� ������ ������
for j=j_min+1:j_max,
    if (pict4(1,j) > 0),
        k = string_prev(j-1);
        if (k > 0),
            string_prev(j) = k;
            N_elem(k) = N_elem(k) + 1;
            clust{k}(1:2,N_elem(k))= [1; j];
        else
            N_clust = N_clust + 1;
            string_prev(j) = N_clust;
            N_elem(N_clust) = 1;
            clust{N_clust}(1:2,1) = [1; j];
        end
    end
end

% �������� ��� ������� ������
string_new = zeros(1,j_max);

% �������� ���� ��������� �����
for i=i_min+1:i_max,
    
    % ����� ������� ��������� ������ �������
    if (pict4(i,j_min) > 0),
        
        % ���� ������ ��� ���� �������
        if (string_prev(j_min) > 0),
            string_new(j_min) = string_prev(j_min);
        end
        if (string_prev(j_min+1) > 0),
            string_new(j_min) = string_prev(j_min+1);
        end
    end
    
    % �������� ��������� ��������� � ������
    for j=j_min+1:j_max,
        if (pict4(i,j) > 0)
            
            % ���� ������ ����� � ����� ������ ���� �������
            if (string_prev(j-1) > 0)
                string_new(j) = string_prev(j-1);
            end
            if (string_prev(j) > 0)
                string_new(j) = string_prev(j);
            end
            
            % ���� ����� ������ ������ ���� �������
            if (string_new(j-1) > 0)
                string_new(j) = string_new(j-1);
            end
            
            % ���� ������� ���� ������ ������
            if ((j<j_max) && (string_prev(j+1)>0))
                
                % ���� ������� ����� ��� �� ��������
                % �� � ������ ��������, �� �������
                % �� � ����, ��� ������ ������
                if ((string_new(j)==0) || (string_new(j)==string_prev(j+1)))
                    string_new(j) = string_prev(j+1);
                else
                    
                    % � ���� ���, �� ��������� ������� ��
                    % ��������������� ���������, �����
                    % � ����� �� �����
                    if (check(equiv,[string_new(j) string_prev(j+1)])==0)
                        equiv_count=equiv_count+1;
                        equiv{equiv_count}=[string_new(j) string_prev(j+1)];
                    end
                end
            end
            
            % �� � ���� �� ���� �� ���������� ��������
            % �� ���������, �� ������� ����� �������
            if (string_new(j) == 0),
                N_clust = N_clust+1;
                N_elem(N_clust) = 0;
                string_new(j) = N_clust;
            end
            
            % ��� ��������� ���������� � ������� ��������
            % � ���� ���������� � �������
            k = string_new(j);
            N_elem(k) = N_elem(k)+1;
            clust{k}(1:2,N_elem(k)) = [i; j];
        end
    end
    string_prev = string_new;
    string_new = zeros(size(string_new));
end

% �������� ��������� ������������ ��������� �� ��������� ���,
% � ����� ������ ������ ���� ���������, � ������� ���� �������������
[list,diffs]=merge(equiv);

% � ��� ������ ���������, � ������� ��� �������������
tmp1=setdiff(1:N_clust,diffs);

% ����� ������� ������������� ��������
final=[];
for i=1:size(list,2)
    tmp=[];
    for j=1:size(list{i},2)
        tmp=[tmp clust{list{i}(:,j)}];
    end
    final{i}=tmp;
end

% ... � ��������� "��������"
for i=1:length(tmp1)
    final{length(final)+1}=clust{tmp1(i)};
end

% ��������� �������������� �������, ���������� ��������,
% � ����� �������� �� �������, ������� ��� �� ���������
[clust,chars]=ellipses(final,pict4);

% ��������� ���������� � ���������� ��������� � ��������� � ������ �� ���
N_clust=length(clust);
N_elem=[];
for i=1:N_clust
    N_elem(i)=size(clust{i},2);
end

% ������ ������� �� �����������
img=draw_ellipses(chars,i_max,j_max);
