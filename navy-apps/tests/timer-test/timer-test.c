#include <stdio.h>

int main() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int ms = 500;
  while (1) {
    gettimeofday(&tv, NULL);
    if (tv.tv_usec==0 ||tv.tv_usec==500000) {
    printf("time %d ", tv.tv_sec);
    printf("ms = %d\n", ms);
    ms += 500;
  }
}
}