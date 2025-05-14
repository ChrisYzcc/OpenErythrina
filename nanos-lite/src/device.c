#include "debug.h"
#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i++) {
    putch(((char *)buf)[i]);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) {
    return 0;
  }
  size_t wr_len = strlen(keyname[ev.keycode]) + 3;
  if (wr_len <= len) {
    sprintf(buf, "k%c %s\n", ev.keydown ? 'd' : 'u', keyname[ev.keycode]);
    return wr_len;
  }
  return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  return sprintf(buf, "WIDTH:%d\nHEIGHT:%d\n", w, h);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;

  int start_x = offset % w;
  int start_y = offset / w;

  int end_x = (offset + len / 4 >= w * h) ? w - 1 : (offset + len / 4) % w;
  int end_y = (offset + len / 4 >= w * h) ? h - 1 : (offset + len / 4) / w;

  int idx = 0;
  for (int y = start_y; y <= end_y; y++) {
    int wr_len;
    if (y == start_y) {
      wr_len = w - start_x;
    } else if (y == end_y) {
      wr_len = end_x + 1;
    } else {
      wr_len = w;
    }
    io_write(AM_GPU_FBDRAW, start_x, y, (void *)((uint32_t *)buf + idx), wr_len, 1, false);
    idx += wr_len;
  }
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  
  return offset + len / 4 > w * h ? w * h - offset : len / 4;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
