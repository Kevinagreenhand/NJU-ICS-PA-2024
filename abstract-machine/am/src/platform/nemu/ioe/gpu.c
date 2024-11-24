#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {

}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t tmp = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = tmp>>16, .height = tmp & 0xffff,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  //if (ctl->sync) {
    //outl(SYNC_ADDR, 1);
  //}
  if (!ctl->sync && (ctl->w == 0 || ctl->h == 0)) return;
  uint32_t *pixels = ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t tmp = inl(VGACTL_ADDR) >> 16;
  for (int i = ctl->y; i < ctl->y+ctl->h; i++) {
    for (int j = ctl->x; j < ctl->x+ctl->w; j++) {
      *(fb+tmp*i+j) = *(pixels+ctl->w*(i-ctl->y)+(j-ctl->x));
    }
  }
  
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
