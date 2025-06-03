#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <stdint.h>

// Function to read the PEB (Process Environment Block) of the current process
// gcc.exe -m64 -o debugging debugging.c
#define DEBUG 0

void* get_teb_64(){
    void* teb;
    __asm__ __volatile__("mov %%gs:0x30, %0" : "=r"(teb));
    return teb;
}

void* get_peb_64() {
    void* teb = get_teb_64();
    void* peb;
    __asm__ __volatile__("mov %%gs:0x30, %%rax\n\t" 
                         "mov 0x60(%%rax), %0"
                         : "=r" (peb) 
                         : 
                         : "rax");
    return peb;
}

void* get_debuginfo_x64() {
    void* teb = get_teb_64();
    void* debug;
    __asm__ __volatile__("mov %[teb], %%rax\n\t"
                         "mov 0x60(%%rax), %%rax\n\t"
                         "mov 0x02(%%rax), %[debug]"
                         :[debug] "=r"(debug)
                         :[teb] "r" (teb)
                         : "rax");
    return debug;
}

int isdebugging_x64() {
    int debug = (BYTE)get_debuginfo_x64();
    if(debug==0) {
        return FALSE; // Not debugging
    } else {
        return TRUE; // Debugging is enabled
    }
}

int main() {
#if DEBUG
    printf("TEB address: %p\n", get_teb_64());
    printf("PEB address: %p\n", get_peb_64());
    printf("Debug Info address: %d\n", (BYTE)get_debuginfo_x64());
#endif

   if (isdebugging_x64()) {
        printf("Debugging is enabled.\n");
    } else {
        printf("Debugging is not enabled.\n");
    }

}