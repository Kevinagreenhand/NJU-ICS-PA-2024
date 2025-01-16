#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
//to enanble ramdisk_read to be recognised
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  Elf_Ehdr ehdr;
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
  Elf_Phdr phdr[ehdr.e_phnum];
  //注，针对lseek的宏不知为何失效，这里的0是SEEK_SET
  fs_lseek(fd, ehdr.e_phoff, 0);
  fs_read(fd, phdr, sizeof(Elf_Phdr)*ehdr.e_phnum);
  //没找到assert的魔数到底是多少，暂时不按指导书加assert
  for (int i = 0; i < ehdr.e_phnum; i++) {
    if (phdr[i].p_type == PT_LOAD) {
      fs_lseek(fd, phdr[i].p_offset, 0);
      fs_read(fd, (void *)phdr[i].p_vaddr, phdr[i].p_memsz);
      memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
    }
  }
  return ehdr.e_entry;


  //TODO();
  //return 0;
}
uintptr_t loader_to_use_outside(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  Elf_Ehdr ehdr;
  fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
  Elf_Phdr phdr[ehdr.e_phnum];
  //注，针对lseek的宏不知为何失效，这里的0是SEEK_SET
  fs_lseek(fd, ehdr.e_phoff, 0);
  fs_read(fd, phdr, sizeof(Elf_Phdr)*ehdr.e_phnum);
  //没找到assert的魔数到底是多少，暂时不按指导书加assert
  for (int i = 0; i < ehdr.e_phnum; i++) {
    if (phdr[i].p_type == PT_LOAD) {
      fs_lseek(fd, phdr[i].p_offset, 0);
      fs_read(fd, (void *)phdr[i].p_vaddr, phdr[i].p_memsz);
      memset((void*)(phdr[i].p_vaddr+phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
    }
  }
  return ehdr.e_entry;


  //TODO();
  //return 0;
}
void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

