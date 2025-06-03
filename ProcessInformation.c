#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <stdint.h>

// Function to read the PEB (Process Environment Block) of the current process
// gcc.exe -m64 -o ImagePath ImagePath.c

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

void* get_procpara_x64() {
    void* peb = get_peb_64();
    void* procpara;
    __asm__ __volatile__("mov %[peb], %%rdx\n\t"
                         "mov 0x20(%%rdx), %[procpara]"
                         :[procpara] "=r"(procpara)
                         :[peb] "r" (peb)
                         : "rax");
    return procpara;
}

void* get_pcurdir(){
    void* procpara = get_procpara_x64();
    void* pcurdir;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x40(%%rax), %%rdx\n\t"
                         "mov %%rdx , %[pcurdir]"
                         :[pcurdir] "=r"(pcurdir)
                         :[procpara] "r" (procpara)
                         : "rax", "rdx");
    return pcurdir;
}

void* get_pcommandline(){
    void* procpara = get_procpara_x64();
    void* pcommandline;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x78(%%rax), %[pcommandline]"
                         :[pcommandline] "=r"(pcommandline)
                         :[procpara] "r" (procpara)
                         : "rax");
    return pcommandline;
}

void* get_pimagepathname(){
    void* procpara = get_procpara_x64();
    void* pimagepathname;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x78(%%rax), %[pimagepathname]"
                         :[pimagepathname] "=r"(pimagepathname)
                         :[procpara] "r" (procpara)
                         : "rax");
    return pimagepathname;
}

void* get_pdllName(){
    void* procpara = get_procpara_x64();
    void* pdllName;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x58(%%rax), %[pdllName]"
                         :[pdllName] "=r"(pdllName)
                         :[procpara] "r" (procpara)
                         : "rax");
    return pdllName;
}

void* get_pwindowTitle(){
    void* procpara = get_procpara_x64();
    void* pwindowTitle;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0xb8(%%rax), %[pwindowTitle]"
                         :[pwindowTitle] "=r"(pwindowTitle)
                         :[procpara] "r" (procpara)
                         : "rax");
    return pwindowTitle;
}

int main() {
#if DEBUG    
    printf("TEB address: %p\n", get_teb_64());
    printf("PEB address: %p\n", get_peb_64());
    printf("ProcPara address: %p\n",get_procpara_x64());
#endif
    printf("Command Line: %ls\n", get_pcommandline());
    printf("Image Pathname: %ls\n", get_pimagepathname());
    printf("DLL Name: %ls\n", get_pdllName());
    printf("Window Title: %ls\n", get_pwindowTitle());
    printf("Current Directory: %ls\n", get_pcurdir());
    
    return 0; 
}