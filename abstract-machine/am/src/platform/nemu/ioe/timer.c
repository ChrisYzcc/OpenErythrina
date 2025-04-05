#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  volatile uint32_t hb = inl(RTC_ADDR + 4);
  volatile uint32_t lb = inl(RTC_ADDR);
  volatile uint64_t res = (uint64_t)(((uint64_t)hb << 32) | lb);
  uptime->us = res;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
