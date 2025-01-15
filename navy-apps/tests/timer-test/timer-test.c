#include <stdio.h>

int main() {
  uint32_t tmp=0;
  int flag=0;
  while (1) {
    tmp=NDL_GetTicks();
    if (tmp%500==0&&flag==0) {
    printf("time %d\n ", tmp);
    flag= 1;
  }
  if(tmp%500!=0){
    flag=0;
  }
}
}