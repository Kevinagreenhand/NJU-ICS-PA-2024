#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
//to enable naive_load to be recognized
extern void naive_uload(PCB *pcb, const char *filename);
void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
  switch_boot_pcb();
  //对助教老师。我没有在loader里加fs文件操作，因此请更改navy-apps里的Makefile来运行测试
  naive_uload(NULL, NULL);
  Log("Initializing processes...");
  // load program here

}

Context* schedule(Context *prev) {
  return NULL;
}
