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
  char key[64]={0};
  char state[2],name[15];
  memset(key,0,sizeof(key));
  key[0] = '0';
  int keycode = 0;
  int ret=NDL_PollEvent(key,sizeof(key));
  if(key[0]=='0'){
    ev->key.keysym.sym = SDLK_NONE;
    ev->type = SDL_USEREVENT;
    return 0;
  }
  sscanf(key,"%s %s\n",state,name);
  ev->key.type = ev->type =(state[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
  for (int i = 0; i < 83; i++) {
    if (!strcmp(keyname[i], name)) {
      ev->key.keysym.sym = i;
      keystate[i] = (state[1] == 'd') ? 1 : 0;
      break;
    }
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
  uint8_t type = 0, sym = 0;
  //SDL_PumpEvents();

  //while (!pop(&type, &sym)){
  while (!read_keyinfo(&type, &sym)){
    //SDL_PumpEvents();
  }
  
  event->type = type;
  event->key.keysym.sym = sym;

  switch(type){
    case SDL_KEYDOWN:
      key_state[sym] = 1;
      break;
    
    case SDL_KEYUP:
      key_state[sym] = 0;
      break;
  }
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
