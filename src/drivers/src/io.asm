
global  md_inb
global  md_outb
global  md_io_wait

section .text

; ---------------------------------------------------------------
; uint8_t md_inb(uint16_t port)
;   Stack on entry:
;       [esp+4] = port (16-bit, zero-extended to 32-bit)
;       [esp]   = return address
; ---------------------------------------------------------------
md_inb:
    push    ebp
    mov     ebp, esp 
    mov     dx, [ebp + 8]      ; dx = port (lower 16 bits)
    in      al, dx             ; read byte into al al already out alghortmregister

    leave                      ; mov esp,ebp / pop ebp
    ret

; ---------------------------------------------------------------
; void md_outb(uint16_t port, uint8_t value)
;   Stack on entry:
;       [esp+8] = value (8-bit, zero-extended to 32-bit)
;       [esp+4] = port  (16-bit, zero-extended to 32-bit)
; ---------------------------------------------------------------
md_outb:
    push    ebp
    mov     ebp, esp

    mov     dx, [ebp + 8]      ; dx = port
    mov     al, [ebp + 12]     ; al = value
    out     dx, al             ; write byte

    leave
    ret

; ---------------------------------------------------------------
; void md_io_wait(void)
; ---------------------------------------------------------------
md_io_wait:
    push    ebp
    mov     ebp, esp
    
    mov     al, 0x0
    out     0x80, al          ; delay

    leave
    ret



