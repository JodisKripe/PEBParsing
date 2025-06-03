#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <stdint.h>

// Function to read the PEB (Process Environment Block) of the current process
// gcc.exe -m64 -o Modules Modules.c

#define DEBUG 1

void* get_teb_64(){
    void* teb;
    __asm__ __volatile__("mov %%gs:0x30, %0" : "=r"(teb));
    return teb;
}

void* get_peb_64() {
    void* teb = get_teb_64();
    void* peb;
    __asm__ __volatile__("mov %[teb], %%rax\n\t" 
                         "mov 0x60(%%rax), %[peb]"
                         :[peb] "=r" (peb) 
                         :[teb] "r" (teb) 
                         : "rax");
    return peb;
}

void* get_ldr_x64() {
    void* peb = get_peb_64();
    void* ldr;
    __asm__ __volatile__("mov %[peb], %%rdx\n\t"
                         "mov 0x18(%%rdx), %[ldr]"
                         :[ldr] "=r"(ldr)
                         :[peb] "r" (peb)
                         : "rax");
    return ldr;
}


int main() {
#if DEBUG    
    printf("TEB address: %p\n", get_teb_64());
    printf("PEB address: %p\n", get_peb_64());
#endif
    void* ldr = get_ldr_x64();
#if DEBUG
    printf("LDR address: %p\n", ldr);
    if (ldr == NULL) {
        printf("LDR is NULL\n");
    } else {
        printf("LDR is not NULL\n");
    }
#endif
    
    return 0; 
}