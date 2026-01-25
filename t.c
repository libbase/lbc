#include <clibp.h>


// x86
typedef enum {
    eax = 0xB8,
    ecx = 0xB9,
    edx = 0xBA,
    ebx = 0xBB,
    esp = 0xBC,
    ebp = 0xBD,
    esi = 0xBE,
    edi = 0xBF
} mov;

typedef struct {
    u8 opcode;
    const string x86;
    const string x64;
} reg_info;

#define REG_COUNT 8
reg_info REGISTERS[] = {
    { eax, "eax", "rax" },
    { ebx, "ebx", "rbx" },
    { ecx, "ecx", "rcx" },
    { edx, "edx", "rdx" },
    { ebp, "ebp", "rbp" },
    { esp, "esp", "rsp" },
    { edi, "edi", "rdi" },
    { esi, "esi", "rsi" }
};

u8 *mov_imm32_reg(mov reg, u64 n) {
    return (u8 []){ reg, n & 0xFF, (n >> 8) & 0xFF, (n >> 16) & 0xFF, (n >> 24) & 0xFF };
}

u8 *mov_imm64_reg(mov reg, u64 n) {
    return (u8 []){ 0x48, n, n & 0xFF, (n >> 8) & 0xFF, (n >> 16) & 0xFF, (n >> 24) & 0xFF, (n >> 32) & 0xFF, (n >> 40) & 0xFF, (n >> 48) & 0xFF, (n >> 56) & 0xFF };
}

// x86_64
u8 *invoke_syscall() {
    return (u8 []){ 0x0f, 0x05 };
}

u8 *invoke_0x80() {
    return (u8 []){ 0xCD, 0x80 };
}

typedef struct 
{
    u8              *code;     // raw bytes
    int             needs_ptr; // on 'mov reg, raw_ptr'
    int             bytes;     // Bytes in 'code'
} opc;

opc OpCodes[1024] = {0};
int OpCodeCount = 0;

bool is_number(string q)
{
    for(int i = 0; q[i] != '\0'; i++)
    {
        if(q[i] < 0 && q[i] > 9)
            return false;
    }

    return true;
}

mov reg_to_type(string reg)
{
    for(int i = 0; i < REG_COUNT; i++)
    {
        if(str_cmp(REGISTERS[i].x86, reg) || str_cmp(REGISTERS[i].x64, reg))
            return REGISTERS[i].opcode;
    }

    return -1;
}

opc convert_asm(string q, ptr p)
{
    if(!q || !p)
        return (opc){0};

    int argc = 0;
    sArr args = NULL;
    if(find_char(q, ' ') > -1)
        args = split_string(q, ' ', &argc);

    // mov reg, 3
    // mov reg, 0x005320ad0402
    if(str_startswith(q, "mov"))
    {
        if(argc < 3)
            clibp_panic("Invalid opcode...!");

        // Pointer Detected
        if(str_startswith(args[2], "0x"))
        {
            mov reg = reg_to_type(args[1]);
            string value = args[2]; // This needs to be checked for max number of i32
            // OpCodes[OpCodeCount++] = (opc){
            //     .code = N <= 0x7FFFFFFF ? mov_imm32_reg(reg, value) : mov_imm64_reg(reg, value),
            //     .needs_ptr = 0,
            //     .bytes = 3
            // };
        }
        
        // Pushing a number to register (Must detect max number for imm32 or imm64)
        if(is_number(args[2]))
        {
            mov reg = reg_to_type(args[1]);
            u64 value = args[2][0] + '0'; // This needs to be checked for max number of i32
            OpCodes[OpCodeCount++] = (opc){
                .code = value <= 0x7FFFFFFF ? mov_imm32_reg(reg, value) : mov_imm64_reg(reg, value),
                .needs_ptr = 0,
                .bytes = 3
            };
        }
    }

    // lea reg, [VARIABLE]
    if(str_startswith(q, "lea"))
    {
        if(argc < 3)
            clibp_panic("Invalid opcode...!");

        // Variable detected ()
        if(str_startswith(args[2], "[") && str_endswith(args[2], "]"))
        {
            
        }
    }

    // syscall
    if(!str_startswith(q, "syscall"))
    {

    }

    return (opc){0};
}

// opc create_op_code_line(u8 *opcode, int bytes) {
//     return (opc){
//         .code = {},
//         .needs_ptr
//     };
// }

i8 entry()
{
    
    return 0;
}