#include <stdio.h>

int main() {
  int ms = 500;
  uint32_t tmp=0;
  int flag=0;
  while (1) {
    tmp=NDL_GetTicks();
    if (tmp%500==0&&flag==0) {
    printf("time %d\n ", tmp);
    ms += 500;
    flag= 1;
  }
  if (tmp%500==0&&flag==1) {
    printf("time %d\n ", tmp);
    flag= 0;
  }
  if(tmp%500!=0){
    flag=0;
  }
}
}