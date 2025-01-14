#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  //新加入
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

extern int ramdisk_read(void *buf, int offset, int len);
extern int ramdisk_write(const void *buf, int offset, int len);
int fs_open(const char *pathname, int flags, int mode){
  //自动忽略前三个特殊文件
  for(int i=3;i<sizeof(file_table)/sizeof(Finfo);i++){
    if(strcmp(file_table[i].name,pathname)==0){
      file_table[i].open_offset=0;
      return i;
    }
  }
  assert(0);
}

int fs_close(int fd){
  return 0;
}
size_t fs_read(int fd, void *buf, size_t len){
  if(fd<=2)
    return 0;
  //注，这里假设offset不会越界,也就是在本次调用之前所有的offset被正确处理
  if(file_table[fd].open_offset+len>file_table[fd].size){
      len=file_table[fd].size-file_table[fd].open_offset;
  }
  ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset+=len;
  return len;
}

size_t fs_write(int fd, const void *buf, size_t len){
  if(fd==0)
    return 0;
  else if(fd<=2){
    for(int i=0;i<len;i++)
      putch(*((char *)buf+i));
    return len;
  }
  if(file_table[fd].open_offset+len>file_table[fd].size){
      len=file_table[fd].size-file_table[fd].open_offset;
  }
  ramdisk_read((void*)buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset+=len;
  return len;
}
size_t fs_lseek(int fd, size_t offset, int whence) {
    if(fd<=2)
      return 0;
    if(whence==SEEK_SET){
      assert(offset <= file_table[fd].size);
      file_table[fd].open_offset = offset;
    }
    else if(whence==SEEK_CUR){
      assert(file_table[fd].open_offset+ offset <= file_table[fd].size);
      file_table[fd].open_offset = file_table[fd].open_offset+ offset;
    }
    else if(whence== SEEK_END){
      assert(file_table[fd].size + offset <= file_table[fd].size);
      file_table[fd].open_offset =  file_table[fd].size + offset;
    }
    else
      assert(0);
    return file_table[fd].open_offset;
}