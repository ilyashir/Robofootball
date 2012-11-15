function res=check_1dim(list,elem)
res=false;
n=length(list);
for i=1:n
    if (list(i)==elem)
        res=true;
        break;
    end
end