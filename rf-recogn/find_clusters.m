%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                       %%%%
%%%% Функция для выделения кластеров       %%%%
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

% Количество кластеров
N_clust = 0;
% Вектор Количества элементов в каждом кластере
N_elem=[];

% Для хранения пар эквивалентных кластеров
equiv=[];
equiv_count=0;

% Здесь хранятся значения для предыдущей строки
string_prev = zeros(1,j_max);

% Сами кластеры
clust=[];

% Проверка первого элемента в первой строке (мало ли там чево)
if (pict4(i_min,j_min) > 0),
    N_clust = 1;
    string_prev(j_min) = N_clust;
    N_elem(N_clust) = 1;
    clust{1}(1:2,1) = [1; 1];
end

% Проверка остальных элементов первой строки
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

% Значения для текущей строки
string_new = zeros(1,j_max);

% Проверка всех остальных строк
for i=i_min+1:i_max,
    
    % Опять сначала проверяем первый элемент
    if (pict4(i,j_min) > 0),
        
        % Если сверху уже есть кластер
        if (string_prev(j_min) > 0),
            string_new(j_min) = string_prev(j_min);
        end
        if (string_prev(j_min+1) > 0),
            string_new(j_min) = string_prev(j_min+1);
        end
    end
    
    % Проверка остальных элементов в строке
    for j=j_min+1:j_max,
        if (pict4(i,j) > 0)
            
            % Если сверху слева и точно сверху есть кластер
            if (string_prev(j-1) > 0)
                string_new(j) = string_prev(j-1);
            end
            if (string_prev(j) > 0)
                string_new(j) = string_prev(j);
            end
            
            % Если перед данной точкой есть кластер
            if (string_new(j-1) > 0)
                string_new(j) = string_new(j-1);
            end
            
            % Если кластер есть справа сверху
            if ((j<j_max) && (string_prev(j+1)>0))
                
                % Если текущая точка еще не отнесена
                % ни к какому кластеру, то относим
                % ее к тому, что сверху справа
                if ((string_new(j)==0) || (string_new(j)==string_prev(j+1)))
                    string_new(j) = string_prev(j+1);
                else
                    
                    % А если нет, то добавляем отметку об
                    % эквивалентности кластеров, чтобы
                    % в конце их слить
                    if (check(equiv,[string_new(j) string_prev(j+1)])==0)
                        equiv_count=equiv_count+1;
                        equiv{equiv_count}=[string_new(j) string_prev(j+1)];
                    end
                end
            end
            
            % Ну а если ни одна из предыдущих проверок
            % не сработала, то заводим новый кластер
            if (string_new(j) == 0),
                N_clust = N_clust+1;
                N_elem(N_clust) = 0;
                string_new(j) = N_clust;
            end
            
            % Тут добавляем информацию о размере кластера
            % и сами координаты в кластер
            k = string_new(j);
            N_elem(k) = N_elem(k)+1;
            clust{k}(1:2,N_elem(k)) = [i; j];
        end
    end
    string_prev = string_new;
    string_new = zeros(size(string_new));
end

% Получаем множества эквиалентных кластеров из множества пар,
% а также просто номера всех кластеров, у которых есть эквивалентные
[list,diffs]=merge(equiv);

% А тут номера кластеров, у которых нет эквивалентных
tmp1=setdiff(1:N_clust,diffs);

% Далее сливаем эквивалентные кластера
final=[];
for i=1:size(list,2)
    tmp=[];
    for j=1:size(list{i},2)
        tmp=[tmp clust{list{i}(:,j)}];
    end
    final{i}=tmp;
end

% ... и добавляем "одиночек"
for i=1:length(tmp1)
    final{length(final)+1}=clust{tmp1(i)};
end

% Вычисляем характеристики эллипов, содержащих кластеры,
% а также отсекаем те эллипсы, которые нам не подходяты
[clust,chars]=ellipses(final,pict4);

% Обновляем информацию о количестве кластеров и элементов в каждом из них
N_clust=length(clust);
N_elem=[];
for i=1:N_clust
    N_elem(i)=size(clust{i},2);
end

% Рисуем эллипсы на изображении
img=draw_ellipses(chars,i_max,j_max);
