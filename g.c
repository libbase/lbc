#include <libbase.h>

extern unsigned char _binary_fag_bin_start[];
extern unsigned char _binary_fag_bin_end[];

int entry() {
    size_t size = _binary_fag_bin_end - _binary_fag_bin_start;
    char byte[3];
    _printf("Size: %d\n", (void *)&size);
    
    long ret = __syscall__(0, _HEAP_PAGE_, PROT_READ | PROT_WRITE | PROT_EXEC, 0x02 | 0x20 | 0x40 , -1, 0, _SYS_MMAP);
    if(ret < 0)
        println("failed to create execution buffer!");
    
    void *heap = (void *)ret;
    mem_cpy(heap, _binary_fag_bin_start + 6, size);
    
    for(int i = 0, nl = 0; i < size; i++)
    {
        if(i == nl + 5)
        {
            nl += 5;
            println(NULL);
        }

        byte_to_hex(((unsigned char *)heap)[i], byte);
        _printf(i == nl - 5 ? "%s" : "%s, ", byte);
    }
    void (*fag)(void) =
    (void (*)(void))heap + 6;
    fag();
    return 0;
}
