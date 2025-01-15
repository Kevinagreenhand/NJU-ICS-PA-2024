#include <stdio.h>

int main() {
  int ms = 500;
  uint32_t tmp=0;
  while (1) {
    tmp=NDL_GetTicks();
    if (tmp%500==0) {
    printf("time %d ", tmp);
    ms += 500;
  }
}
}