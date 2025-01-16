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
    Log("Hello World from Nanos-lite with arg '%d' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}
void context_kload(PCB *pcb, void (*entry)(void *), void *arg) {
  pcb->cp = kcontext((Area){pcb->stack, pcb->stack + 32768}, entry, arg);//8*4096,其实是栈的大小
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, (void*)114514);
  context_kload(&pcb[1], hello_fun, (void*)1919810);
  switch_boot_pcb();
  //naive_uload(NULL, "/bin/dummy");
  //Log("Initializing processes...");
  // load program here

}

Context* schedule(Context *prev) {
  //return NULL;
  current->cp = prev;
  printf("%d",current);
  current = ((current == &pcb[0]) ? &pcb[1] : &pcb[0]);
 
  // then return the new context
  return current->cp;
}

