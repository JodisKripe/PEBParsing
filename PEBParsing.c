#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <stdint.h>

// Function to read the PEB (Process Environment Block) of the current process
// gcc.exe -m64 -o PEBParsing PEBParsing.c

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

void* get_procpara_x64() {
    void* teb = get_teb_64();
    void* procpara;
    __asm__ __volatile__("mov %[teb], %%rax\n\t"
                         "mov 0x60(%%rax), %%rdx\n\t"
                         "mov 0x20(%%rdx), %[procpara]"
                         :[procpara] "=r"(procpara)
                         :[teb] "r" (teb)
                         : "rax");
    return procpara;
}

void* get_envaddr_x64(){
    void* procpara = get_procpara_x64();
    void* envaddr;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x80(%%rax), %[envaddr]"
                         :[envaddr] "=r"(envaddr)
                         :[procpara] "r" (procpara)
                         : "rax");
    return envaddr;
}

void* get_envsize_x64() {
    void* procpara = get_procpara_x64();
    void* envsize;
    __asm__ __volatile__("mov %[procpara], %%rax\n\t"
                         "mov 0x3f0(%%rax), %[envsize]"
                         :[envsize] "=r"(envsize)
                         :[procpara] "r" (procpara)
                         : "rax");
    return envsize;
}

void* get_first_string(){
    void* envaddr = get_envaddr_x64();
    void* first_string = NULL;
    __asm__ __volatile__("mov %[envaddr], %%rax\n\t"
                         "mov $0, %%rcx\n\t"
                         "looper: \n\t"
                         "inc %%rcx\n\t"
                         "cmpb $0, (%%rcx,%%rax,1)\n\t"
                         "jne looper\n\t"
                         "inc %%rcx\n\t"
                         "cmpb $0, (%%rcx,%%rax,1)\n\t"
                         "jne looper\n\t"
                         "mov %%rcx, %[first_string]"
                         :[first_string] "=r"(first_string)
                         :[envaddr] "r" (envaddr)
                         : "rax", "rcx", "rdx");
    return first_string;
}

void* get_string_length(void* envaddr) {
    void* first_string = NULL;
    __asm__ __volatile__("mov %[envaddr], %%rax\n\t"
                         "mov $0, %%rcx\n\t"
                         "looper2: \n\t"
                         "inc %%rcx\n\t"
                         "cmpb $0, (%%rcx,%%rax,1)\n\t"
                         "jne looper2\n\t"
                         "inc %%rcx\n\t"
                         "cmpb $0, (%%rcx,%%rax,1)\n\t"
                         "jne looper2\n\t"
                         "mov %%rcx, %[first_string]"
                         :[first_string] "=r"(first_string)
                         :[envaddr] "r" (envaddr)
                         : "rax", "rcx", "rdx");
    return first_string;
}

void count_down(){
    int i=10;
    int j=0;
    __asm__ __volatile__("countdown: \n\t"
                         "dec %[i]\n\t"
                         "cmp $5, %[i]\n\t"
                         "jne countdown\n\t"
                         "mov %[i], %[j]"
                         :[j] "+r"(j), [i] "+r"(i)
                         :
                         :);
    printf("Countdown finished: %d\n", i);
}

void loopStrings(){
    PVOID envaddr = get_envaddr_x64();
    int env_size = (int)get_envsize_x64();
    PVOID i = envaddr;
    int first_string_len = (int)get_first_string();
    while(i < envaddr + (int)env_size) {
        int string_length = (int)get_string_length((void*)i);
        if (string_length == 0) {
            break; // End of strings
        }
        printf("String at %p: %.*ls\n", (void*)i, string_length, (wchar_t*)i);
        i += string_length + 2; // Move to the next string (+2 for null terminators wide characters)
    }


}

int main() {
    printf("TEB address: %p\n", get_teb_64());
    printf("PEB address: %p\n", get_peb_64());
    printf("ProcPara address: %p\n",get_procpara_x64());
    printf("EnvAddr address: %p\n", get_envaddr_x64());
    printf("EnvSize : %p\n", get_envsize_x64());
    printf("First String length: %d\n",get_first_string());
    // void* addr = (PVOID)get_envaddr_x64() + (unsigned __int64)get_first_string();
    printf("First String: %ls\n", get_envaddr_x64());
    PVOID envaddr = get_envaddr_x64();
    PVOID nextString = envaddr + (int)get_first_string() + 2; // +1 to skip the null terminator
    printf("Next String: %ls\n", nextString);
    loopStrings();
}