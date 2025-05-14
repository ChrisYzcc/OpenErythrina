#include "sdl-event.h"
#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char buf[64];
  int len = NDL_PollEvent(buf, 64);

  if (len <= 0) {
    return 0;
  }

  buf[len] = '\0';
  char _keyname[64];
  char state;
  sscanf(buf, "k%c %s\n", &state, _keyname);
  if (state == 'd') {
    ev->type = SDL_KEYDOWN;
  }
  else if (state == 'u') {
    ev->type = SDL_KEYUP;
  }
  else {
    return 0;
  }
  
  for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++) {
    if (strcmp(_keyname, keyname[i]) == 0) {
      ev->key.keysym.sym = i;
      break;
    }
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  int len;
  while ((len = NDL_PollEvent(buf, 64)) <= 0);

  buf[len] = '\0';
  char _keyname[64];
  char state;
  sscanf(buf, "k%c %s\n", &state, _keyname);
  if (state == 'd') {
    event->type = SDL_KEYDOWN;
  }
  else if (state == 'u') {
    event->type = SDL_KEYUP;
  }
  else {
    return 0;
  }
  
  for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++) {
    if (strcmp(_keyname, keyname[i]) == 0) {
      event->key.keysym.sym = i;
      break;
    }
  }
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
