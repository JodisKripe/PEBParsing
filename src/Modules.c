#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <stdint.h>

// Function to read the PEB (Process Environment Block) of the current process
// gcc.exe -m64 -o Modules Modules.c

#define DEBUG 0

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

void* inMemoryOrderModuleList_x64() {
    void* ldr = get_ldr_x64();
    void* inMemoryOrderModuleList;
    __asm__ __volatile__("mov %[ldr], %%rax\n\t"
                         "mov 0x20(%%rax), %[inMemoryOrderModuleList]"
                         :[inMemoryOrderModuleList] "=r"(inMemoryOrderModuleList)
                         :[ldr] "r" (ldr)
                         : "rax");
    return inMemoryOrderModuleList;
}


int main() {
#if DEBUG  
    printf("ntdll.dll address: %p\n", GetModuleHandleA("ntdll.dll"));
    printf("Kernel32.dll address: %p\n", GetModuleHandleA("kernel32.dll"));
    printf("KernelBase.dll address: %p\n", GetModuleHandleA("kernelbase.dll"));
    printf("Ucrtbase.dll address: %p\n", GetModuleHandleA("user32.dll"));
    printf("TEB address: %p\n", get_teb_64());
    printf("PEB address: %p\n", get_peb_64());
#endif
    
#if DEBUG
    void* ldr = get_ldr_x64();
    printf("LDR address: %p\n", ldr);
    if (ldr == NULL) {
        printf("LDR is NULL\n");
    } else {
        printf("LDR is not NULL\n");
    }
#endif
    void* inMemoryOrderModuleList = inMemoryOrderModuleList_x64();
#if DEBUG
    printf("InMemoryOrderModuleList address: %p\n", inMemoryOrderModuleList);
    if (inMemoryOrderModuleList == NULL) {
        printf("InMemoryOrderModuleList is NULL\n");
    } else {
        printf("InMemoryOrderModuleList is not NULL\n");
    }
#endif
    
    PLIST_ENTRY head = (PLIST_ENTRY)inMemoryOrderModuleList;
    PLIST_ENTRY current = head->Flink;
    printf("Modules in memory order:\n");
    int count = 0;
    while (current != head) {
        LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        if(count == 0) {
            printf("NTDLL: ");
        } else if(count == 1) {
            printf("KERNEL32: ");
        } else if(count == 2) {
            printf("KERNELBASE: ");
        } else if(count == 3) {
            printf("UCRTBASE: ");
        }
        #if DEBUG
        printf("Module: %ls, Base Address: %p\n", entry->FullDllName.Buffer, entry->DllBase);
        #else
        printf(": %p\n", entry->DllBase);
        #endif
        current = current->Flink;
        count++;
    }

    return 0; 
}