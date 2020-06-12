// Liburanium // Licensed under the Academic Free License (AFL 3.0)

#include "../types.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

class VgaTextMode
{
    public:
        static const uint8_t x=0,y=0;
        static constexpr uint16_t* VideoMemory = (uint16_t*)0xb8000;
        static const bool cleared = false;
}

VgaTextMode textMode;

void printf(char* str)
{
    //static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    //public static uint8_t x=0,y=0;

    if textMode.cleared=true
    {
        textMode.x=0;
        textMode.y=0;
    }
    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                textMode.x = 0;
                textMode.y++;
                break;
            default:
                textMode.VideoMemory[80*y+x] = (textMode.VideoMemory[80*y+x] & 0xFF00) | str[i];
                textMode.x++;
                break;
        }

        if(textMode.x >= 80)
        {
            textMode.x = 0;
            textMode.y++;
        }

        if(textMode.y >= 25)
        {
            for(textMode.y = 0; textMode.y < 25; textMode.y++)
                for(textMode.x = 0; textMode.x < 80; textMode.x++)
                    textMode.VideoMemory[80*y+x] = (textMode.VideoMemory[80*y+x] & 0xFF00) | ' ';
            textMode.x = 0;
            textMode.y = 0;
        }
    }
}

void clear()
{
    //static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    // public static uint8_t x=0,y=0;

    for(textMode.y = 0; textMode.y < 25; textMode.y++)
        for(textMode.x = 0; textMode.x < 80; textMode.x++)
            textMode.VideoMemory[80*textMode.y+textMode.x] = (textMode.VideoMemory[80*textMode.y+textMode.x] & 0xFF00) | ' ';
    
    textMode.x = 0;
    textMode.y = 0;
    printf("clear run")
}

printf("liburanium exists")