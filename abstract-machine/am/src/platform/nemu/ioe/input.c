#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t kbd_addr_res = inl(KBD_ADDR);
  kbd->keycode = kbd_addr_res & (~KEYDOWN_MASK);
  kbd->keydown = (kbd_addr_res & KEYDOWN_MASK ? true : false);
}