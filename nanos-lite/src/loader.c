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
static uintptr_t loader(PCB *pcb, const char *filename) {
      Elf_Ehdr ehdr;
      ramdisk_read(&ehdr, 0, sizeof(Elf_Ehdr));
      //没找到assert的魔数到底是多少，暂时不按指导书加assert
      Elf_Phdr phdr[ehdr.e_phnum];
      ramdisk_read(phdr, ehdr.e_phoff, sizeof(Elf_Phdr)*ehdr.e_phnum);
      for (int i = 0; i < ehdr.e_phnum; i++) {
      if (phdr[i].p_type == PT_LOAD) {
          ramdisk_read((void*)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
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

