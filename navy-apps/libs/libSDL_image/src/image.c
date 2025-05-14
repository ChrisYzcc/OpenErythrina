#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  assert(filename != NULL);
  FILE *fp = fopen(filename, "rb");
  assert(fp != NULL);

  // get size
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);

  // allocate buffer
  uint8_t *buf = SDL_malloc(size);
  assert(buf != NULL);
  fseek(fp, 0, SEEK_SET);
  fread(buf, 1, size, fp);

  SDL_Surface *surface = STBIMG_LoadFromMemory(buf, size);
  SDL_free(buf);
  fclose(fp);

  return surface;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
