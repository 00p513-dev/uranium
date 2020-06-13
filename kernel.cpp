#include "liburanium.h"
#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "driver.h"
#include "keyboard.h"

// Test push by TheAwesome98 (you can delete this comment)

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}





class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

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

    printf("\n");

    //please dont write shit beneath here unless you know why! i wasted an hour of my life because of this shit
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    printf("GDT and InterruptManager loaded! Let's get some drivers! \n");

    printf("Initializing Hardware, Stage 1\n");

    DriverManager drvManager;

        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDriver(&keyboard);

    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");

    while(1);
}
