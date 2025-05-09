#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <NDL.h>

int main() {
  uint32_t start_ms= NDL_GetTicks();
  while (1) {
    uint32_t now_ms = NDL_GetTicks();
    if (now_ms - start_ms >= 500) {
      printf("0.5 second passed\n");
      start_ms = now_ms;
    }
  }
  return 0;
}
