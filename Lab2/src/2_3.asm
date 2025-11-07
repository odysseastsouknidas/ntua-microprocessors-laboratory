.include "m328PBdef.inc"    
; Interrupt vector table
.org 0x0
rjmp MAIN
.org 0x4
rjmp INT1_ISR
.equ X1 = 1000

MAIN:
    ; Set up external interrupt INT1
    ldi r16, (1 << ISC11) | (1 << ISC10)  ; Set rising edge detection on INT1 (EICRA)
    sts EICRA, r16                        ; Enable rising edge on INT1

    ldi r16, (1 << INT1)                  ; Enable INT1 interrupt (EIMSK)
    out EIMSK, r16

    sei                                   ; Enable global interrupts

    ; Set PORTB and PORTC as output
    ldi r16, 0xFF                         ; Set DDRB = 0xFF (all outputs)
    out DDRB, r16
LOOP:
    ; Infinite loop, set PORTB = 0x00
    ldi r16, 0x00                         ; Clear PORTB (PORTB = 0x00)
    out PORTB, r16
    rjmp LOOP                             ; Repeat indefinitely

   
INT1_ISR:
	sei
recheck:
    ldi r26, (1 << INTF1)
    out EIFR, r26
    ldi r24, low(100)
    ldi r25, high(100)
    rcall wait_x_ms // delay 100ms
    in r26, EIFR
    sbrc r26,INTF0      //wait why not INTF1??
    rjmp recheck
    ; Check if PORTB == 0x01 or PORTB == 0xFF
    in r16, PORTB                         ; Load PORTB value into r16
    cpi r16, 0x01                         ; Compare with 0x01
    breq TURN_ON_ALL_LEDS                 ; If equal, go to TURN_ON_ALL_LEDS
    cpi r16, 0xFF                         ; Compare with 0xFF
    breq TURN_ON_ALL_LEDS                 ; If equal, go to TURN_ON_ALL_LEDS
TURN_ON_PB0:
    ldi r16, 0x01                         ; Set PB0 (PORTB = 0x01)
    out PORTB, r16
    ldi r24, low(5000)           
    ldi r25, high(5000)                   ; Wait for 500 ms
    rcall wait_x_ms
    ldi r16, 0x00                         ; Turn off PB0 (PORTB = 0x00)
    out PORTB, r16
    reti                                  ; Return from interrupt

TURN_ON_ALL_LEDS:
    ldi r16, 0xFF                         ; Set all LEDs on PORTB (PORTB = 0xFF)
    out PORTB, r16

    ldi r24, low(500)           
    ldi r25, high(500)
    rcall wait_x_ms
    ldi r16, 0x01  
    out PORTB, r16
    ldi r24, low(4500)           
    ldi r25, high(4500)              ; Wait for 50 ms
    rcall wait_x_ms
    ldi r16, 0x00                    ; Turn off all LEDs (PORTB = 0x00)
    out PORTB, r16
    reti                             ; Return from interrupt

wait_x_ms:
    ldi r16, 16
extra_outer_delay:
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
;this routine is used to produce a delay of (1000*X1) cycles
delay_outer:
    push r24              ; (2 cycles)
    push r25              ; (2 cycles) Save r24:r25
loop4:
    rcall delay_inner     ; (3+993)=996 cycles
    sbiw r24,1            ; 2 cycles
    brne loop4            ; 1 or 2 cycles
    pop r25               ; (2 cycles)
    pop r24               ; (2 cycles) Restore r24:r25
    ret                   ; 4 cycles
