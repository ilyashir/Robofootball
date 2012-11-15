%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                %%%%
%%%% Функция для вычисления характеристик эллипсов, %%%%
%%%% содержащих кластеры, а также отсечение         %%%%
%%%% "плохих" кластеров                             %%%%
%%%%                                                %%%%
%%%% @author: "RoboFootball SPbSU team"             %%%%
%%%% @email: robofootball@gmail.com                 %%%%
%%%%                                                %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [clusters, chars]=ellipses(clusters,img)

chars=[];
step = 0;
i=1;

 while (i<=length(clusters))  
    
    m=0.0;
    xc=0.0;
    yc=0.0;
    ix=0.0;
    iy=0.0;
    ixy=0.0;
    
    % Вычисление цетров масс кластеров
    for j=1:size(clusters{i},2)
        x=clusters{i}(1,j);
        y=clusters{i}(2,j);
        mi=double(img(x,y));
        m=m+mi;
        xc=xc+x*mi;
        yc=yc+y*mi;
    end
    
    xc=xc/m;
    yc=yc/m;
    
    % Вычисление моментов инерции
    for j=1:size(clusters{i},2)
        x=clusters{i}(1,j);
        y=clusters{i}(2,j);
        mi=double(img(x,y));
        
        ix=ix+mi*(x-xc)^2;
        iy=iy+mi*(y-yc)^2;
        ixy=ixy+mi*(x-xc)*(y-yc);
    end
    ix=ix/m;
    iy=iy/m;
    ixy=ixy/m;
    
    % Вычисление большой и малой полуоси,
    % а также угла поворота
    [a,b,phi]=ellipse_calc(ix,iy,ixy);
    
    % Проверка параметров
    if (a * b > 2) && (max(a,b)/min(a,b) < 2) && (a * b < 60)
      step = step + 1;
      if (a<b)
          a=b;
      end
      chars{step}=[xc yc 2*a];
    else
      clusters(i)=[];  
      i=i-1;
    end;
  i=i+1;    
end