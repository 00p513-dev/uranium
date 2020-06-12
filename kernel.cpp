#include "liburanium.h"
#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World!");
    printf("more text that'll be erased");

    clear();

    printf("Welcome to the Uranium Kernel!\n");
    printf("\n");
    printf("pid check! \n");
    int pid = getpid();
    char cpid = '0' + pid;
    char* dpid = &cpid;
    printf(dpid);

    //please dont write shit beneath here, i wasted an hour of my life because of this shit
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    KeyboardDriver keyboard(&interrupts);
    interrupts.Activate();

    while(1);
}
