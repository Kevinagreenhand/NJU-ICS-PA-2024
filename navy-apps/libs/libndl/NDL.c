#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
//新加入的
static int canvas_w = 0, canvas_h = 0;

uint32_t NDL_GetTicks() {
  //由于初始化的时间可以很短，因此这里不对时间的修正，也就是没有在init的时候初始化，不减去init的时间
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  //注意，这里应该是直接open。若错，后改
  int fd = open("/dev/events", 0, 0);
  int tmp = read(fd, (void*)buf, len);
  if(tmp==0)
    return 0;
  else
    return 1;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  if(*w==0&&*h==0)
    *w = screen_w; *h = screen_h;
  canvas_w = *w; canvas_h = *h;
    
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb", 0, 0);
  for (int i = 0; i < h && y + i < canvas_h; i++) {
    printf("%d\n",((y + (screen_h-canvas_h)/2 + i) * screen_w + (x + (screen_w-canvas_w)/2)));
    lseek(fd, ((y + (screen_h-canvas_h)/2 + i) * screen_w + (x + (screen_w-canvas_w)/2)) * 4, SEEK_SET);
    write(fd, pixels + i * w, 4 * (w < canvas_w - x ? w : canvas_w - x));
  }

}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  int screen_fd = open("/proc/dispinfo",0,0);
  char buf[64];
  read(screen_fd, buf, 64);
  sscanf(buf, "WIDTH:%d\nHEIGHT:%d\n", &screen_w, &screen_h);
  return 0;
}

void NDL_Quit() {

}
