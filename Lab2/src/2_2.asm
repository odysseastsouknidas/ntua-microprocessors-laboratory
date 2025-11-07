.include "m328PBdef.inc"
    
.equ FOSC_MHZ = 16
.equ DEL_mS = 2000
.equ DEL_NU = FOSC_MHZ*DEL_mS 

; Interrupt vector table
.org 0x0
rjmp reset
.org 0x2
rjmp ISR0

reset:
    ; initialize stack pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24

    ; Init PORTC as output, PORTB as input
    ser r26
    out DDRC, r26
    clr r26
    out DDRB, r26
    
    ;Interrupts on rising edge of INT0 pin
    ldi r24, (1<< ISC01) | (1<<ISC00)
    sts EICRA, r24

    ; Enabld the INT0 interrupt (PD2)
    ldi r24, (1<<INT0)
    out EIMSK, r24
    sei ; enable interrupts


loop1:
    clr r26
loop2:
    andi r26, 0x1F
    out portc, r26
    ldi r24, low(DEL_NU)
    ldi r25, high(DEL_NU) ; set delay
    rcall delay_ms
    
    inc r26
    cpi r26,32
    breq loop1
    rjmp loop2
    
    

 ISR0:
    push r26
    push r25
    push r24
    in r24, SREG ; save r24, r25, SREG
    push r24 ; push to stack

recheck:
    ldi r24, (1 << INTF0)
    out EIFR, r24
    ldi r24, low(16*5)
    ldi r25, high(16*5)
    rcall delay_ms // delay 5ms
    in r24, EIFR
    sbrc r24,INTF0
    rjmp recheck
    
    ldi r24, (1 << INTF0)
    out EIFR, r24 // clear interrupt flag
    cli // disable interrupts (temporarily)
    
    
    ldi r24, 0 ; in r24 we will produce the output for PORTC
    in r25, PINB
    ldi r16, 4 ; this is a counter in order to see only up to bit PB3

count_in_b_loop:
    mov r28, r25
    andi r28, 0x01 ; keep only LSB
    cpi r28, 0x00 ; if LSB is 0 (pressed) increment r24
    breq to_inc
    rjmp continue

to_inc:
    lsl r24
    ori r24, 0x01 ; push bits left and make the LSB 1 
    
continue:
    lsr r25 ; bring second LSB to LSB
    dec r16
    brne count_in_b_loop    
    
output_leds:
    out portc, r24
    
    ldi r24, low(DEL_NU)
    ldi r25, high(DEL_NU)
    rcall delay_ms ; delay
    
    
    
    
    pop r24
    out SREG, r24
    pop r24
    pop r25
    pop r26 ; pop from stack
    
    reti


/* delary routine */
delay_ms:
    ldi r23, 249
loop_inner:
    dec r23
    nop
    brne loop_inner
    
    sbiw r24,1
    brne delay_ms
    ret



