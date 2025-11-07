.include "m328PBdef.inc"

.def counter=r21
.def temp=r22
.equ FOSC_MHZ = 16
.equ DEL_mS = 1000
.equ DEL_NU = FOSC_MHZ*DEL_mS 
    
.org 0x000
    rjmp reset
.org 0x004
    rjmp isr1
    
reset:
    /*    Initialise stack    */
    ldi temp, LOW(RAMEND)
    out SPL, temp
    ldi temp, HIGH(RAMEND)
    out SPH, temp
    
    /*    PORTD input */
    clr temp
    out DDRD, temp
    
    /*    PORTB and PORTC output    */
    ser temp
    out DDRC, temp
    out DDRB, temp
    
    /* zero counter */
    clr counter
    
    /*    Setup ISR1 interrupt on high edge   */
    ldi temp, (1 << ISC11 | 1 << ISC10)
    sts EICRA, temp
    
    /*    Enable ISR1 interrupt    */
    ldi temp, (1 << INT1)
    out EIMSK, temp
    
    /*    Enable general interrupt flag    */
    sei
    
main0:    
loop1:
    clr r26
loop2:
    out portb, r26
    ldi r24, low(DEL_NU)
    ldi r25, high(DEL_NU) ; set delay
    rcall delay_routine
    
    inc r26

    cpi r26,64
    breq loop1
    rjmp loop2
/*  Interrupt handler    */
isr1:
    push r26
    push r25
    push r24
    in r24,SREG
    push r24 ; save on stack
 
recheck:    ; to avoid debouncing
    ldi r24, (1 << INTF1)
    out EIFR, r24
    ldi r24, low(5*16)
    ldi r25, high(5*16) ; set delay
    rcall delay_routine // delay 5ms
    in r24, EIFR
    sbrc r24,INTF0
    rjmp recheck
    
    ldi temp, (1 << INTF1)
    out EIFR, temp // clear interrupt flag
    cli // disable interrupts (temporarily)
    
    in temp, PIND
    sbrc temp, 05    // if PD5 is pressed skip next line
    inc counter
    out PORTC, counter // turn lights up
    
    pop r24
    out SREG, r24
    pop r24
    pop r25
    pop r26 ; pop from stack
    
    sei // re-enable interrupts
    reti // return from interrupt

/*  Delay routine   */
delay_routine:
    ldi r23, 249
loop_inner:
    dec r23
    nop
    brne loop_inner
    sbiw r24,1
    brne delay_routine
    ret

