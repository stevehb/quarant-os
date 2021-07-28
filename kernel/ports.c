#include <stdint.h>

// Low level port I/O functions
// These functions are used by most hardware drivers in our kernel
#include "ports.h"

// Read a byte from the specified port
unsigned char port_byte_in(unsigned short port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}

// Write a byte into the specified port
void port_byte_out(unsigned short port, unsigned char data) {
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

// Read a word from the specified port
unsigned short port_word_in(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}

// Write a word into the specified port
void port_word_out(unsigned short port, unsigned short data) {
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}




uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}
void outb(uint16_t port, uint8_t data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}
void outw(uint16_t port, uint16_t data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
