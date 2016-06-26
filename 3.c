#include <stdio.h>

int prodescalar(int *vec1, int *vec2, dim)
{
  if(dim == 0)
    return 1
  else
    return (vec1--)*(vec2--) + prodescalar(vec1,vec2,dim--);
}

main(){
  int a;
  int vec1={2,2,2}, vec2={1,1,1};
  a = prodescalar(vec1, vec2, 3);
}
