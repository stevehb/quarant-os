#include <stdbool.h>
#include <stdint.h>
#include "dbgserial.h"
#include "ports.h"
#include "string.h"


static const uint16_t COM1 = 0x03f8;
static bool is_init = false;

// From https://wiki.osdev.org/Serial_Ports
int dbg_init_serial() {
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(COM1 + 0) != 0xAE) {
        is_init = false;
        return 1;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1 + 4, 0x0F);
    is_init = true;
    return 0;
}

void dbg_print_c(char c) {
    if(is_init == false) { return; }
    int tx_is_empty = 0;
    while(tx_is_empty == 0) { 
        tx_is_empty = inb(COM1 + 5) & 0x20; 
    }
    outb(COM1, c);
}

void dbg_print(const char* str) {
    int i = 0;
    while(str[i] != '\0') {
        dbg_print_c(str[i]);
        i++;
    }
}

void dbg_printi(int i) {
    char buff[80] = { 0 };

    bool is_neg = i < 0;
    if(is_neg) { i = -i; }

    int idx = 0;
    do {
        int rem = i % 10;
        buff[idx] = '0' + (uint8_t) rem;
        i = i / 10;
    } while(i > 0);

    strrev(buff);
    dbg_print(buff);
}
