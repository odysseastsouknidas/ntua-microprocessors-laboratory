.include "m328PBdef.inc"
.equ X1 =110         ; here we put the desired mS of delay
    
; initialize stack pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24

    ldi r24, low(X1)          ; 
    ldi r25, high(X1)         ; Set delay

loop1:
    rcall wait_x_msec      ; cycles_needed = 16*x*1000
    rjmp loop1
    
wait_x_msec:
    ldi r16, 16
extra_outer_delay:     ; 16 times the delay_outer
    rcall delay_outer
    subi r16,1
    brne extra_outer_delay
    ret
;this routine is used to produce a delay 993 cycles
delay_inner:
    ldi r23, 247          ; 1 cycle
loop3:
    dec r23               ; 1 cycle
    nop                   ; 1 cycle
    brne loop3            ; 1 or 2 cycles
    nop                   ; 1 cycle
    ret                   ; 4 cycles

delay_outer:              ; X times the 993 cycles + overhead
    push r24              ; (2 cycles)
    push r25              ; (2 cycles) Save r24:r25

loop4:
    rcall delay_inner     ; (3+993)=996 cycles
    sbiw r24,1            ; 2 cycles
    brne loop4            ; 1 or 2 cycles

    pop r25               ; (2 cycles)
    pop r24               ; (2 cycles) Restore r24:r25
    ret                   ; 4 cycles


