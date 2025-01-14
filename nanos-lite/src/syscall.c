#include <common.h>
#include "syscall.h"

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

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
  case 4:c->GPRx=fs_write(a[1],(void*)a[2],a[3]);break;
  case 9:c->GPRx=0;break;
  case 2:c->GPRx=fs_open((char*)a[1],a[2],a[3]);break;
  case 3:c->GPRx=fs_read(a[1],(void*)a[2],a[3]);break;
  case 7:c->GPRx=fs_close(a[1]);break;
  case 8:c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

