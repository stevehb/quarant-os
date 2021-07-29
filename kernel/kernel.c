#include <stdint.h>

// #include "function.h"
#include "kernel.h"
#include "isr.h"
// #include "screen.h"
// #include "string.h"


#include "font.h"
#include "dbgserial.h"
#include "ports.h"


const uint32_t WIDTH = 320;
const uint32_t HEIGHT = 200;
uint8_t* const vidptr = (uint8_t*) 0xA0000;



void font_print_char(char c, uint32_t x, uint32_t y) {
    const uint8_t* rows = FONT[(uint8_t) c];
    int offset = 0;
    for(int yy = 0; yy < 8; yy++) {
        const uint8_t cell = rows[yy];
        for(int xx = 0; xx < 8; xx++) {
            offset = ((y + yy) * WIDTH) + (x + xx);
            if(cell & (1 << xx)) {
                vidptr[offset] = 0x3C;
            } else {
                vidptr[offset] = 0x00;
            }
        }
    }
}


void set_pal(uint8_t clr, uint8_t r, uint8_t g, uint8_t b) {
    outb(0x03C8, clr);  // Color index
    outb(0x03C9, r);  // R 
    outb(0x03C9, g);  // G
    outb(0x03C9, b);  // B
}

void k_main() {
    // clear_screen();
    isr_install();
    irq_install();
    dbg_init_serial();


    // Set greyscale palette
    for(uint32_t i = 0; i < 256; i++) {
        uint8_t val = (i - (i % 4)) / 4;
        set_pal(i, val, val, val);
    }


    for(uint32_t y = 0; y < HEIGHT; y++) {
        for(uint32_t x = 0; x < 256; x++) {
            uint32_t offset = (y * WIDTH) + x;
            vidptr[offset] = (uint8_t) x;
        }
    }


    // Test floating point
    dbg_print("\n\nTesting floating point math\n");
    dbg_print("8 = '"); dbg_printi(8); dbg_print("'\n");
    dbg_print("456 = '"); dbg_printi(456); dbg_print("'\n");

    float test_a = 1.5f;
    float test_b = 4.f * test_a;
    int test_c = (int) test_b;
    dbg_print("output(6)=");
    dbg_printi(test_c);
    dbg_print("\n");


    // Draw a beautiful tapestry of the default palette
    // int tile_count = 16;
    // int tile_size = HEIGHT / tile_count;  // 12
    // int init_x_off = (WIDTH - (tile_size * tile_count)) / 2;
    // int init_y_off = (HEIGHT - (tile_size * tile_count)) / 2;

    // int offset = 0;
    // for(int y_tile = 0; y_tile < tile_count; y_tile++) {
    //     int y_off = init_y_off + (y_tile * tile_size);
    //     for(int x_tile = 0; x_tile < tile_count; x_tile++) {
    //         int x_off = init_x_off + (x_tile * tile_size);
    //         int pal_idx = (y_tile * tile_count) + x_tile;
    //         for(int yy = 0; yy < tile_size; yy++) {
    //             for(int xx = 0; xx < tile_size; xx++) {
    //                 offset = ((y_off + yy) * WIDTH) + (x_off + xx);
    //                 vidptr[offset] = pal_idx;
    //             }
    //         }
    //     }
    // }


    // font_print_char('A', 50, 50);
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
