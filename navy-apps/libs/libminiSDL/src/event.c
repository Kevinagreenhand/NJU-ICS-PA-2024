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
  char buf[64];

  if (NDL_PollEvent(buf, 64) == 0) return 0;

  else {

    char keytype[4];

    char keycode[32];

    sscanf(buf, "%s %s", keytype, keycode);

    if (strcmp(keytype, "kd") == 0) ev->type = SDL_KEYDOWN;

    else if (strcmp(keytype, "ku") == 0) ev->type = SDL_KEYUP;


    for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++){

      if (strcmp(keycode, keyname[i]) == 0){

        ev->key.type = ev->type;

        ev->key.keysym.sym = i;

        assert(i != 0);

        return 1;

      }

    }

    assert(0); 

  }

  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];

  int ret;

  while ((ret = NDL_PollEvent(buf, 64)) != 0){

    char keytype[4];

    char keycode[32];

    sscanf(buf, "%s %s", keytype, keycode);

    if (strcmp(keytype, "kd") == 0) event->type = SDL_KEYDOWN;

    else if (strcmp(keytype, "ku") == 0) event->type = SDL_KEYUP;

    else assert(0);

    for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++){

      if (strcmp(keycode, keyname[i]) == 0){

        //printf("SDL_WaitEvent ret: %d %s\n", ret, keycode);

        event->key.type = event->type;

        event->key.keysym.sym = i;

        assert(i != 0);

        return 1;

      }

    }

    assert(0); 

  }

  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
