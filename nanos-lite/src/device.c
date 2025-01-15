#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for(int i=0;i<len;i++)
    putch(*((char*)buf+i));
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T keyboard_message = io_read(AM_INPUT_KEYBRD);
  char *readhelp=(char*)buf;
  if(keyboard_message.keycode==AM_KEY_NONE){
    *readhelp='\0';
    return 0;
  }
  else{
    if(keyboard_message.keydown){
      *readhelp++='k';
      *readhelp++='d';
      *readhelp++=' ';
      for(int i=0;i<strlen(keyname[keyboard_message.keycode]);i++)
        *readhelp++=keyname[keyboard_message.keycode][i];
      *readhelp++='\n';
      *readhelp='\0';
    }
    else{
      *readhelp++='k';
      *readhelp++='u';
      *readhelp++=' ';
      for(int i=0;i<strlen(keyname[keyboard_message.keycode]);i++)
        *readhelp++=keyname[keyboard_message.keycode][i];
      *readhelp++='\n';
      *readhelp='\0';
    }
    return readhelp-(char*)buf;
  }
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
