#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
//to enable naive_load to be recognized
extern void naive_uload(PCB *pcb, const char *filename);
extern uintptr_t loader_to_use_outside(PCB *pcb, const char *filename);
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
void context_uload(PCB *pcb, const char *filename) {
  unsigned long entry = loader_to_use_outside(pcb, filename);
  pcb->cp = ucontext(NULL, (Area){pcb->stack, pcb->stack + 32768}, (void*)entry);
  pcb->cp->GPRx = (unsigned long) heap.end;
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, (void*)114514);
  context_uload(&pcb[1], "/bin/bird");
  switch_boot_pcb();
  //naive_uload(NULL, "/bin/dummy");
  //Log("Initializing processes...");
  // load program here

}

Context* schedule(Context *prev) {
  //return NULL;

  //printf("%d\n",current);
  //if(current==&pcb[0]){
    //printf("switch to pcb[1]");
    //pcb[0].cp = prev;
    //current = &pcb[1];
  //printf("%d\n",current);
    //return pcb[1].cp;
  //}
  //else{
    //printf("switch to pcb[0]");
    //pcb[1].cp = prev;
    //current = &pcb[0];
  //printf("%d\n",current);
    //return pcb[0].cp;
  //}

  //由于神秘原因，使用if else会出现&pcb[0]和&pcb[1]之外的第三个值？
  //初步推测应该和PCB类型的union有关，换用没问题的写法
  current->cp = prev;
  current = ((current == &pcb[0]) ? &pcb[1] : &pcb[0]);
  return current->cp;
}

