#include <common.h>
#include "syscall.h"
//为了gettimeofday
//#include <sys/_timeval.h>
#include <sys/time.h>
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  long tmp=io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec=tmp/1000000;
  tv->tv_usec=tmp%1000000;
  return 0;
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
  case 4:c->GPRx=fs_write(a[1],(void*)a[2],a[3]);break;
  case 9:c->GPRx=0;break;
  case 2:c->GPRx=fs_open((char*)a[1],a[2],a[3]);break;
  case 3:c->GPRx=fs_read(a[1],(void*)a[2],a[3]);break;
  case 7:c->GPRx=fs_close(a[1]);break;
  case 8:c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
  case 19:c->GPRx=sys_gettimeofday((struct timeval*)a[1],(struct timezone*)a[2]);break;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

