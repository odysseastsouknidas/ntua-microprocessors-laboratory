#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
volatile uint32_t time= 0;

ISR (INT1_vect) {
    time = 0;
    _delay_ms(1);
    sei(); //re-enabling interrupt for nested interrupts to be allowed
    if(PORTB == 0x01 || PORTB == 0x3F) {
        PORTB = 0x3F;
        while(time<500) {
            _delay_ms(1);
            time++;
        }
        PORTB = 0x01;
        while (time< 5000) {
            _delay_ms(1);
            time++;
        }
        PORTB = 0x00;
    }
    else {
        PORTB = 0x01;
        while (time < 5000) {
            _delay_ms(1);
            time++;
        }
        PORTB = 0x00;  
    }
    EIFR = (1<<INTF1); //do i need this?
}

int main() {
    // interrupt on rising edge of INT1
    EICRA = (1<<ISC11) | (1<<ISC10);
    //Enable the INT1 (PD3)
    EIMSK = (1<<INT1);
    sei(); //enable interrupts
    DDRB = 0xFF; //PortB as output
    
    while(1) {
        PORTB = 0x00;
    }
    return 0;
}
            