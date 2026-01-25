#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// x86
typedef enum {
    eax = 0xB8,
    ebx = 0xBB,
    ecx = 0xB9,
    edx = 0xBA,
    ebp = 0xBD,
    esp = 0xBC,
    edi = 0xBF,
    esi = 0xBE
} mov;

uint8_t *mov_imm32_reg(mov reg, uint32_t n) {
    return (uint8_t []){ reg, n & 0xFF, (n >> 8) & 0xFF, (n >> 16) & 0xFF, (n >> 24) & 0xFF };
}

uint8_t *mov_imm64_rax(mov reg, uint64_t n) {
    return (uint8_t []){ 0x48, n, n & 0xFF, (n >> 8) & 0xFF, (n >> 16) & 0xFF, (n >> 24) & 0xFF, (n >> 32) & 0xFF, (n >> 40) & 0xFF, (n >> 48) & 0xFF, (n >> 56) & 0xFF };
}

// x86_64
uint8_t *invoke_syscall() {
    return (uint8_t []){ 0x0f, 0x05 };
}

uint8_t *invoke_0x80() {
    return (uint8_t []){ 0xCF, 0x80 };
}

typedef struct 
{
    uint8_t         *code;     // raw bytes
    int             needs_ptr; // on 'mov reg, raw_ptr'
    int             bytes;     // Bytes in 'code'
} opc;

opc OpCodes[1024] = {0};

int convert_asm(char *q, void *ptr)
{
    // mov reg, 3
    // mov reg, 0x005320ad0402
    if(strstr(q, "mov"))
    {
        // Check for address
        
    }

    // lea reg, [VARIABLE]
    if(strstr(q, "lea"))
    {

    }

    // syscall
    if(!strcmp(q, "syscall"))
    {

    }
}

opc create_op_code_line(unsigned char *op) {
    return (opc){

    };
}


int main()
{
    FILE *fp = fopen("test.bin", "wb");
    if(!fp) return 1;

    unsigned char code[] = {
        0xB8, 0x01, 0x00, 0x00, 0x00,     // mov eax, 1  (sys_write)
        0xBF, 0x01, 0x00, 0x00, 0x00,     // mov edi, 1  (stdout)
        0x48, 0x8D, 0x35, 0x10, 0x00, 0x00, 0x00, // lea rsi, [rip+8]
        0xBA, 0x06, 0x00, 0x00, 0x00,     // mov edx, 6
        0x0F, 0x05,                       // syscall

        0xB8, 0x3C, 0x00, 0x00, 0x00,     // mov eax, 60 (exit)
        0x31, 0xFF,                       // xor edi, edi
        0x0F, 0x05,                       // syscall

        'H','e','l','l','o','\n'
    };
    fwrite(code, 1, sizeof(code), fp);
    fclose(fp);
    return 0;
}