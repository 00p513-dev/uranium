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

    printf("not erased!")    ;
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    KeyboardDriver keyboard(&interrupts);
    interrupts.Activate();

    while(1);
}
