AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -m32 -std=c11 -g -ffreestanding -fno-exceptions -Wall -Wextra

SRCDIR=kernel
OBJDIR=obj
BINDIR=bin

K_SRC_ASM := $(wildcard $(SRCDIR)/*.asm)
K_SRC_C := $(wildcard $(SRCDIR)/*.c)
K_OBJS := $(OBJDIR)/kernel_entry.o \
	$(K_SRC_ASM:$(SRCDIR)/%.asm=$(OBJDIR)/%.o) \
	$(K_SRC_C:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


debug:
	$(info K_SRC_ASM = $(K_SRC_ASM))
	$(info K_SRC_C = $(K_SRC_C))
	$(info K_OBS = $(K_OBJS))

all: dirs os-image.bin

run: all
	qemu-system-i386.exe -drive format=raw,file="\\$(shell wslpath -w os-image.bin)"

clean:
	rm -f ./**/*.o
	rm -f ./**/*.elf
	rm -f ./**/*.bin
	rm -f ./*.img
	rm -f ./*.bin

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

os-image.bin: $(BINDIR)/boot.bin $(BINDIR)/kernel.bin
	cat $^ > os-image.bin

$(BINDIR)/boot.bin: boot/boot.asm
	${AS} $< -f bin -o $@

$(BINDIR)/kernel.bin: ${K_OBJS}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	${CC} ${CFLAGS} -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.asm 
	${AS} $< -f elf -o $@

$(OBJDIR)/%.o: boot/%.asm
	${AS} $< -f elf -o $@
