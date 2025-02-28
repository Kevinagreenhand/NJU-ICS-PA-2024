#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t tmp=0;
  tmp=inl(KBD_ADDR);
  kbd->keydown = (((tmp&KEYDOWN_MASK)==0)?false:true);
  kbd->keycode = tmp&~KEYDOWN_MASK;
}
