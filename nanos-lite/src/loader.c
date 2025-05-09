#include "debug.h"
#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);

  Elf32_Ehdr ehdr;
  fs_read(fd, &ehdr, sizeof(ehdr));

  for (int i = 0; i < ehdr.e_phnum; i++) {
    Elf32_Phdr phdr;
    fs_lseek(fd, ehdr.e_phoff + i * sizeof(phdr), SEEK_SET);
    fs_read(fd, &phdr, sizeof(phdr));

    if (phdr.p_type == PT_LOAD) {
      uintptr_t addr = phdr.p_vaddr;
      size_t memsz = phdr.p_memsz;
      size_t filesz = phdr.p_filesz;

      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, (void *)addr, filesz);
      memset((void *)(addr + filesz), 0, memsz - filesz);
    }
  }
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

