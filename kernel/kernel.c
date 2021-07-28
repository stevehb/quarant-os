#include <stdint.h>

// #include "function.h"
#include "kernel.h"
#include "isr.h"
// #include "screen.h"
// #include "string.h"


#include "font.h"
#include "dbgserial.h"


const uint32_t WIDTH = 320;
const uint32_t HEIGHT = 200;
uint8_t* const vidptr = (uint8_t*) 0xA0000;



void scr_print_char(char c, uint32_t x, uint32_t y) {
    const uint8_t* rows = FONT[(uint8_t) c];
    int offset = 0;
    for(int yy = 0; yy < 8; yy++) {
        const uint8_t cell = rows[yy];
        for(int xx = 0; xx < 8; xx++) {
            offset = ((y + yy) * WIDTH) + (x + xx);
            if(cell & (1 << xx)) {
                vidptr[offset] = 0x32;
            } else {
                vidptr[offset] = 0x00;
            }
        }
    }
}




void k_main() {
    // clear_screen();
    isr_install();
    irq_install();

    int serial = dbg_init_serial();

    // uint8_t* vidptr = (uint8_t*) 0xA0000;

    for(int x = 0; x < 100; x++) {
        for(int y = 0; y < 100; y++) {
            int offset = (y * WIDTH) + x;
            if(serial == 0) {
                vidptr[offset] = 0x0A;
            } else {
                vidptr[offset] = 0x0C;
            }
        }
    }

    dbg_print("line one");
    dbg_print(" also line one\n");
    dbg_print("line two\n\n\n");
    dbg_print("line 5");

    scr_print_char('A', 50, 50);
}

void user_input(char *input) {
    dbg_print(input);

    // if (strcmp(input, "END") == 0) {
    //     print("Stopping the CPU. Bye!\n");
    //     __asm__ __volatile__("hlt");
    // }

    // print("You said: ");
    // print(input);
    // print("\n> ");
}
