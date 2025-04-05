#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t ctrl = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = (ctrl >> 16), .height = (ctrl & 0xffff),
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t ctrl = inl(VGACTL_ADDR);
  int w = (ctrl >> 16);

  uint32_t *fb = (uint32_t *)(uintptr_t)(FB_ADDR);
  uint32_t *pi = (uint32_t *)(uintptr_t)(ctl->pixels);
  for (int y = 0; y < ctl->h; y++){
    for (int x = 0; x < ctl->w; x++){
      fb[(x + ctl->x) + (y + ctl->y) * w] = pi[x + y * ctl->w];
    }
  }
  
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
