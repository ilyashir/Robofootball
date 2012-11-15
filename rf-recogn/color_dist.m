function pic = color_dist(new_pic, size2, size1)

pic = zeros(size2,size1,3);

thr = 202; %remove spots where the brightness of each component is less than this value

for i = 1:size2
    for j = 1:size1
        if ((new_pic(i,j,1) < thr) && (new_pic(i,j,2) < thr) && (new_pic(i,j,3) < thr))
            pic(i,j,:) = [0, 0, 0];
        else
            pic(i,j,:) = new_pic(i,j,:);
        end;
    end;
end