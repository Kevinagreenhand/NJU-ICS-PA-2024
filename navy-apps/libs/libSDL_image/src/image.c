#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
extern int open (const char *file,int flags, ...);
extern int read (int fd,void *buf,size_t cnt);
extern long lseek (int fd,long pos,int whence);
SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  int fd = open(filename, 0, 0),filelength = lseek(fd, 0, 2);
  lseek(fd, 0, 0);
  void *imgbuf = malloc(filelength);
  read(fd, imgbuf, filelength);
  SDL_Surface *surfacepointer = STBIMG_LoadFromMemory((char *)imgbuf, filelength);
  free(imgbuf);
  return surfacepointer;
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
