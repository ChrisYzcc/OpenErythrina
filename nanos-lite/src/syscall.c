#include <common.h>
#include "am.h"
#include "debug.h"
#include "syscall.h"

int mm_brk(uintptr_t brk);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield:{
      yield();
      c->GPRx = 0;
      break;
    }
    case SYS_exit:{
      halt(a[1]);
      break;
    }
    case SYS_write:{
      int fd = a[1];
      const char *buf = (const char *)a[2];
      size_t len = a[3];
      if (fd == 1 || fd == 2) {
        for (size_t i = 0; i < len; i++) {
          putch(buf[i]);
        }
        c->GPRx = len;
      } else {
        c->GPRx = -1;
      }
      break;
    }
    case SYS_brk:{
      c->GPRx = mm_brk(a[1]);
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
