%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                                                %%%%
%%%% Функция для отсечения неподходящих цветов      %%%%
%%%%                                                %%%%
%%%% @author: "RoboFootball SPbSU team"             %%%%
%%%% @email: robofootball@gmail.com                 %%%%
%%%%                                                %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [final_rgb]=get_clouds(list_color,npoint,avg)
final_rgb=[];
new_npoint = [0 0 0 0];

for k=1:4
    new_rRGB=[];
    st = 1; 
    while ((npoint(k)-new_npoint(k))>1)    
 

        for j=1:npoint(k)
            rRGB=list_color{k}(j,:);
            mCov = cov(list_color{k});
            ell = (rRGB-avg{k})*inv(mCov)*(rRGB-avg{k})';
            if ( ell < 6)
                new_rRGB(st,:)=rRGB; 
                st = st+1;
            end
        end


        new_npoint(k)=st-1;
        if (npoint(k)-new_npoint(k)<1) 
            break
        else
            new_npoint(k)=npoint(k);
        end
    end
    final_rgb{k}=new_rRGB;
end