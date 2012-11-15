%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                    %%%%
%%%% Функция для отрисовки эллипсов,    %%%%
%%%% содержащих кластеры                %%%%
%%%%                                    %%%%
%%%% @author: "RoboFootball SPbSU team" %%%%
%%%% @email: robofootball@gmail.com     %%%%
%%%%                                    %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function img=draw_ellipses(chars,height,width)
img=zeros(height,width,'uint8');
for i=1:length(chars)

    xc=chars{i}(1);
    yc=chars{i}(2);
    a=chars{i}(3);

    for j=0:0.05:2*pi
        img(1 + uint32(xc+a*cos(j)), 1 + uint32(yc+a*sin(j)))=255;
    end
   
end