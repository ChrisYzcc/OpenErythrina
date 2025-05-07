#include "debug.h"
#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf32_Ehdr ehdr;
  ramdisk_read(&ehdr, 0, sizeof(ehdr));

  for (int i = 0; i < ehdr.e_phnum; i++) {
    Elf32_Phdr phdr;
    ramdisk_read(&phdr, ehdr.e_phoff + i * sizeof(phdr), sizeof(phdr));

    if (phdr.p_type == PT_LOAD) {
      uintptr_t addr = phdr.p_vaddr;
      size_t memsz = phdr.p_memsz;
      size_t filesz = phdr.p_filesz;

      ramdisk_read((void *)addr, phdr.p_offset, filesz);
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

