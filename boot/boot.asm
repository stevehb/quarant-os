;; Boot sector
;; Loads kernel from the disk into memory
;; Switches processor to 32-bit Protected Mode
;; Executes loaded kernel

[org 0x7C00]
[bits 16]

;; Memory offset where our kernel is located
KERNEL_OFFSET equ 0x1000

;; Save the boot drive number
mov [BOOT_DRIVE], dl

;; Update base and stack pointers
mov bp, 0x9000
mov sp, bp

;; Set video mode to 320x200x256
mov ah, 0x00
mov al, 0x13
int 0x10



;;
;; LOAD KERNEL FROM DISK -> MEMORY
;;
mov dh, 15
mov dl, [BOOT_DRIVE]
push dx

;; Prepare to read the disk
;; al = number of sectors to read (1 - 128)
;; ch = track/cylinder number
;; dh = head number
;; cl = sector number
mov ah, 0x02
mov al, dh
mov bx, KERNEL_OFFSET
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
int 0x13
jc disk_read_error

;; Check whether we read the expected number of sectors
pop dx
cmp dh, al
jne disk_read_error



;;
;; SWITCH FROM REAL MODE -> PROTECTED MODE
;;
cli
lgdt [gdt_descriptor]

;; CR0 (Control Register 0), set bit to flag Protection Enable
mov eax, cr0
or eax, 0x1
mov cr0, eax

;; Far jump to 32 bit instructions to ensure CPU is done
jmp CODE_SEG:init_pm

[bits 32]
init_pm:
;; Update segment registers
mov ax, DATA_SEG
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

;; Update base and stack pointers
mov ebp, 0x90000
mov esp, ebp



;;
;; CALL TO KERNEL
;;
sti
call KERNEL_OFFSET
jmp $



[bits 16]
printer:
	pusha
    xor bh, bh
    mov ah, 0x0E
	mov al, [esi]
	inc esi
    cmp al, 0x00
    je printer_end
    int 0x10
    jmp printer
printer_end:  
	popa
	ret

disk_read_error:
	mov esi, DISK_READ_ERROR_MSG
	call printer
	jmp $


;;
;; GDT DATA: https://wiki.osdev.org/Global_Descriptor_Table 
;;
gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

;; Kernel Code Segment
gdt_kernel_code:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0x9A
	db 11001111b
	db 0x00

;; Kernel Data Segment
gdt_kernel_data:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0x92
	db 11001111b
	db 0x00

;; Userland Code Segment
gdt_userland_code:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 11111010b
	db 11001111b
	db 0x00

;; Userland Data Segment
gdt_userland_data:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 11110010b
	db 11001111b
	db 0x00

gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

CODE_SEG equ gdt_kernel_code - gdt_start
DATA_SEG equ gdt_kernel_data - gdt_start

;; Print LF (0x0A) and CR (0x0D)
DISK_READ_ERROR_MSG: db 0x0D, 0x0A, "Disk read error!", 0x0D, 0x0A, 0
BOOT_DRIVE: db 0

times 510 - ($-$$) db 0
dw 0xAA55
