#include <common.h>
#include "syscall.h"


void do_syscall(Context *c) {
  unsigned long a[4];
  a[0] = c->GPR1; 
  a[1] = c->GPR2; 
  a[2] = c->GPR3; 
  a[3] = c->GPR4; 

  switch (a[0]) {
  //case SYS_exit:halt(0);break;
  case 1:yield();c->GPRx=0;break;
  case 0:halt(a[1]);c->GPRx=0;break;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
