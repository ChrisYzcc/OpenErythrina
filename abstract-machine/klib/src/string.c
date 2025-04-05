#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s != '\0'){
    len ++;
    s++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *ori_dest = dst;
  while (*src != '\0'){
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return ori_dest;
}

char *strncpy(char *dst, const char *src, size_t n) {
  for (int i = 0; i < n; i++){
    *(dst+i) = *(src + i);
  }
  *(dst+n) = '\0';
  return dst;
}

char *strcat(char *dst, const char *src) {
  char *ori_dest = dst;
  while (*dst != '\0')  dst++;
  while (*src != '\0'){
    *dst = *src;
    dst++;
    src++;
  }
  return ori_dest;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 == *s2 && !(*s1 == '\0' && *s1 == '\0')){
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  for (int i = 0; i < n; i++){
    if (*(s1 + i) == *(s2 + i) && *(s1 + i) == '\0')  return 0;
    if (*(s1 + i) == *(s2 + i)) continue;
    return *(s1 + i) - *(s2 + i);
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  for (int i = 0; i < n; i++){
    *(char *)(s + i) = (char)c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  for (int i = 0; i < n; i++){
    *(char *)(dst + i) = *(char *)(src + i);
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  for (int i = 0; i < n; i++){
    *(char *)(out + i) = *(char *)(in + i);
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  for (int i = 0; i < n; i++){
    int res = *(char *)(s1 + i) - *(char *)(s2 + i);
    if (res != 0)
      return res;
  }
  return 0;
}

#endif
