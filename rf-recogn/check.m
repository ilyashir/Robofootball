function res=check(list,elem)
res=false;
n=size(list,2);
for i=1:n
    if (list{i}==elem)
        res=true;
        break;
    end
end