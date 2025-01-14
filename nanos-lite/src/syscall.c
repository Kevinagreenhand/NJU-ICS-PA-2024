#include <common.h>
#include "syscall.h"
int sys_write(int fd,char *buf, unsigned long len) {
  //没有考虑写入不全的情况
  if(fd==1||fd==2)
  {
  for (int i = 0; i < len; i ++) 
    putch(*(buf+i));
  return len;
  }
  else 
    return -1;
}
void do_syscall(Context *c) {
  unsigned long a[4];
  a[0] = c->GPR1; 
  a[1] = c->GPR2; 
  a[2] = c->GPR3; 
  a[3] = c->GPR4; 

  switch (a[0]) {
  //真正进行系统调用的地方
  case 1:yield();c->GPRx=0;break;
  case 0:halt(a[1]);break;
  case 4:c->GPRx=sys_write(a[1],(char*)a[2],a[3]);break;
  case 9:c->GPRx=0;break;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

