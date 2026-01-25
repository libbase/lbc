#define _GNU_SOURCE
#include <elf.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd = open("test.o", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    Elf64_Ehdr eh;
    read(fd, &eh, sizeof(eh));

    if (memcmp(eh.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF\n");
        return 1;
    }

    Elf64_Phdr ph;
    lseek(fd, eh.e_phoff, SEEK_SET);
    read(fd, &ph, sizeof(ph));

    if (ph.p_type != PT_LOAD) {
        fprintf(stderr, "No PT_LOAD\n");
        return 1;
    }

    /* Map memory at fixed address */
    void *mem = mmap(
        (void *)ph.p_vaddr,
        ph.p_memsz,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
        -1,
        0
    );

    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    /* Load segment */
    lseek(fd, 0, SEEK_SET);
    read(fd, mem, ph.p_filesz);

    /* Set RX permissions */
    mprotect(mem, ph.p_memsz, PROT_READ | PROT_EXEC);

    close(fd);

    /* Jump to entry */
    void (*entry)() = (void (*)())eh.e_entry;
    entry();

    return 0;
}
