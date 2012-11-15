function [ grayscale ] = GS(pic)
[n, m, k] = size(pic);

for i = 1:n
    for j = 1:m
        grayscale(i,j) = 0.4 * pic(i, j, 1) + 0.4 * pic(i, j, 2) + 0.2 * pic(i, j, 3);
    end;
end;