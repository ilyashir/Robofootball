%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                              %%%%
%%%% Функция для выделения множеств эквивалентных %%%%
%%%% кластеров из списка эквивалетных пар         %%%%
%%%%                                              %%%%
%%%% @author: "RoboFootball SPbSU team"           %%%%
%%%% @email: robofootball@gmail.com               %%%%
%%%%                                              %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [equiv_clusters,diffs]=merge(list)

equiv_clusters=[];
count=0;

% Пока в списке пар есть элементы
while (size(list,2)>0)
    
    % Временная структура для хранения текущего 
    % списка эквивалентных кластеров
    tmp=[list{1}(1)];
    tmp_length=1;
    j=1;
    
    % Для каждого элемента в списке
    while(j<=length(tmp))
        
        % Текущий элемент
        start=tmp(j);
        i=1;
        
        %Просматриваем по всему списку пар
        while (i<=size(list,2))
            
            % Если в паре слева - текущий элемент,
            % то добавляем элемент справа в текущий список
            % эквивалентных класторов.
            % а пару удаляем
            if (list{i}(1)==start)
                if(check_1dim(tmp,list{i}(2))==0)
                    tmp_length=tmp_length+1;
                    tmp(tmp_length)=list{i}(2);
                end
                list(i)=[];
                i=i-1;
            else
                % Аналогично, если элемент - в паре справа
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
    
    % Добавляем список найденных эквивалентных 
    % кластеров в общую структуру
    equiv_clusters{count}=tmp;
end

% А теперь делаем общий список номеров кластеров,
% у которых есть эквивалентные (чтобы потом
% выделить "одиночек"

diffs=[];
count=0;
for i=1:length(equiv_clusters)
    for j=1:size(equiv_clusters{i},2)
        count=count+1;
        diffs(count)=equiv_clusters{i}(j);
    end
end
