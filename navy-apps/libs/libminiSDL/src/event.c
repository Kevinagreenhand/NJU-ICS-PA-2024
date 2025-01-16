#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};
uint8_t keystate[256] = {};
static uint8_t key_state[sizeof(keyname) / sizeof(keyname[0])] = {0};
int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  //这里实现的是读了flappy bird代码之后的简化版本，因为这个游戏只要求识别按键是落下还是抬起
  char buf[64];
  char type[8];
  if (NDL_PollEvent(buf, 0) == 0)
    return 0;
  sscanf(buf, "%s ", type);
  if(*(buf+1)=='u')
    ev->type==SDL_KEYUP;
  else
    ev->type==SDL_KEYDOWN;
  return 1; 
}

int SDL_WaitEvent(SDL_Event *event) {
  //按理说应该实现，但是由于读了flappy bird代码发现不需要，所以没实现
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
