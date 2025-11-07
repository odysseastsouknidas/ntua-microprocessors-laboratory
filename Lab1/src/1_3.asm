.include"m328PBdef.inc"
.org 0x00
.DEF temp=r17
.DEF leds=r18
.equ X1=1000

reset: ; initialize stack pointer
    ldi r24 , low(RAMEND)
    out SPL , r24
    ldi r24 , high(RAMEND)
    out SPH , r24

start:
    ldi leds,0x01 ; prepare for the first time the LSB of the leds to open
    ser temp
    out DDRD,temp ; D as output
    ldi r24, low(X1)        
    ldi r25, high(X1) ; init of the double register for the delay
    clt ; i think this is needed to make sure that the T is 0 at the beginning
    jmp go_left

change_dir_left: ; an extra delay its time we reach the edge + T =0 as we're about to go left
    rcall wait_x_msec 
    clt
    
go_left:
    out PORTD,leds
    sbrc leds,07 ; ignore next if the train has reached the right-most position
    jmp change_dir_right
    rcall wait_X_msec
    lsl leds
    rjmp go_left
change_dir_right: ; an extra delay its time we reach the edge + T =1 as we're about to go left
    rcall wait_X_msec
    set
go_right:
    out PORTD,leds
    sbrc leds,00 ; ignore next if the train has reached the left-most position
    rjmp change_dir_left
    rcall wait_X_msec
    lsr leds
    rjmp go_right

wait_x_msec: ; dealy routine from 1.1
    ldi r16, 16
extra_outer_delay:
    rcall delay_outer
    subi r16,1
    brne extra_outer_delay
    ret
delay_inner:
    ldi r23, 247          ; 1 cycle
loop3:
    dec r23               ; 1 cycle
    nop                   ; 1 cycle
    brne loop3            ; 1 or 2 cycles
    nop                   ; 1 cycle
    ret                   ; 4 cycles

;this routine is used to produce a delay of (1000*Fl+14) cycles
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
   





