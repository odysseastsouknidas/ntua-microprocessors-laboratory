.include "m328PBdef.inc"
.org 0x00
    
.def A= r16
.def B= r17
.def C= r18
.def D= r19
.def F0= r20
.def F1= r21
.def dummy= r22
.def counter= r23

MAIN:

    ldi A, 0x51
    ldi B, 0x41
    ldi C, 0x21
    ldi D, 0x01
    ldi counter, 0x05


loop:
    mov F0,B ; F0 = B
    com F0 ; F0 = B'
    mov dummy, F0 ; dummy = B'
    and F0, A ; F0 = A*B'
    and dummy, D ; dummy = B'*D
    or F0, dummy ; F0 = A*B' + B'*D
    com F0 ; F0 = (A*B' + B'*D)'
    mov F1, C ; F1 = C
    com F1 ; F1 = C'
    or F1, A ; F1 = A+C'
    mov dummy, D ; dummy = D
    com dummy ; dummy = D'
    or dummy, B ; dummy = B+D'
    and F1, dummy ; F1 = (A+C')*(B+D')
    ldi dummy, 0x01
    add A, dummy ; A += 0x01
    ldi dummy, 0x02
    add B, dummy ; B += 0x02
    ldi dummy, 0x03
    add C, dummy ; C += 0x03
    ldi dummy, 0x04
    add D, dummy ; D += 0x04
    dec counter
    brge loop

END:
    rjmp END


