#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    if(c->mcause==-1)
      ev.event=EVENT_YIELD;
    else if (c->mcause>=0&&c->mcause<=19)
    //according to navy-apps/libs/libos/syscall.h
      ev.event=EVENT_SYSCALL;
    else
      ev.event=EVENT_ERROR;
    
    //switch (c->mcause) {
      //case -1: ev.event = EVENT_YIELD; break;
      //default: ev.event = EVENT_ERROR; break;
    //}

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  //return NULL;
  Context *new_context_pointer = (Context *)(kstack.end-sizeof(Context));
  new_context_pointer->mepc=(unsigned long)entry;
  return new_context_pointer;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
