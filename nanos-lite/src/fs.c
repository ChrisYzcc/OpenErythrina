#include <fs.h>
#include <stddef.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_DEV, FD_DISPLAYINFO, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_DEV]    = {"/dev/events", 0, 0, events_read, invalid_write},
  [FD_DISPLAYINFO] = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write},
  [FD_FB]     = {"/dev/fb", 0, 0, invalid_read, fb_write},
#include "files.h"
};

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < sizeof(file_table) / sizeof(Finfo); i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      return i;
    }
  }
  panic("file not found");
}

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t fs_read(int fd, void *buf, size_t len) {
  Finfo *f = &file_table[fd];
  size_t nxt_offset = f->open_offset + len > f->size ? f->size : f->open_offset + len;
  size_t size = nxt_offset - f->open_offset;

  if (f->read != NULL) {
    return f->read(buf, f->disk_offset + f->open_offset, len);
  }

  ramdisk_read(buf, f->disk_offset + f->open_offset, size);
  f->open_offset += size;
  return size;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  Finfo *f = &file_table[fd];
  size_t size = len;

  if (f->write != NULL) {
    size_t wr_len =  f->write(buf, f->disk_offset + f->open_offset, size);
    f->open_offset += wr_len;
    return wr_len;
  } else {
    size_t nxt_offset = f->open_offset + len > f->size ? f->size : f->open_offset + len;
    size = nxt_offset - f->open_offset;
    ramdisk_write(buf, f->disk_offset + f->open_offset, size);
    f->open_offset += size;
  }
  return size;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  Finfo *f = &file_table[fd];
  size_t nxt_offset;
  switch (whence) {
    case SEEK_SET:
      nxt_offset = offset;
      break;
    case SEEK_CUR:
      nxt_offset = f->open_offset + offset;
      break;
    case SEEK_END:
      nxt_offset = f->size + offset;
      break;
    default:
      panic("invalid whence");
  }
  if (nxt_offset > f->size) {
    return -1;
  }
  f->open_offset = nxt_offset;
  return f->open_offset;
}

int fs_close(int fd) {
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  file_table[FD_FB].size = w * h * sizeof(uint32_t);
}
