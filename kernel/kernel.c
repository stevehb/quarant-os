#include "function.h"
#include "kernel.h"
#include "isr.h"
#include "screen.h"
#include "string.h"

void k_main() {
  clear_screen();
  isr_install();
  irq_install();

  print("Type something, it will go through the kernel\n");
  print("Type END to halt the CPU\n> ");
}

void user_input(char *input) {
  if (strcmp(input, "END") == 0) {
    print("Stopping the CPU. Bye!\n");
    __asm__ __volatile__("hlt");
  }

  print("You said: ");
  print(input);
  print("\n> ");
}
