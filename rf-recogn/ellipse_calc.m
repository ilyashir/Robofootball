function [lambda_max,lambda_min,phi] = ellipse_calc(Ix,Iy,Ixy)

if ((Ixy == 0) && (Ix == Iy)),
    lambda_min = sqrt(Ix);
    lambda_max = sqrt(Iy);
    phi = 0;
    return;
end

D = sqrt((Ix-Iy)^2 + 4*Ixy^2);
lambda_min = sqrt((Ix+Iy - D)/2);
lambda_max = sqrt((Ix+Iy + D)/2);

d = (Ix-Iy)/D;
if (d > 0),
    phi = atan(sqrt((1-d)/(1+d)));
else
    phi = pi/2 - atan(sqrt((1+d)/(1-d)));
end
if (Ixy < 0),
    phi = -phi;
end