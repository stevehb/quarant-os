SOURCES = $(shell find cpu drivers include kernel libc -name '*.c')
HEADERS = $(shell find cpu drivers include kernel libc -name '*.h')
OBJ = ${SOURCES:.c=.o cpu/interrupt.o}

ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -std=c11

ifeq ($(shell uname -s),Darwin)
	CC = i386-elf-gcc
	LD = i386-elf-ld
endif

all: os-image.bin

run: all
	qemu-system-i386.exe -drive format=raw,file="C:\Users\stevehb\AppData\Local\Packages\TheDebianProject.DebianGNULinux_76v4gfsz19hv4\LocalState\rootfs\home\stevehb\ghaiklor-os-gcc\os-image.bin"
	# qemu-system-i386 -fda os-image.bin

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o

os-image.bin: boot/boot.bin kernel/kernel.bin
	cat $^ > os-image.bin

boot/boot.bin: boot/boot.asm
	${ASM} $< -f bin -o $@

kernel/kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${ASM} $< -f elf -o $@

%.bin: %.asm
	${ASM} $< -f bin -o $@
