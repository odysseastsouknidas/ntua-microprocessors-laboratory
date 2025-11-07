#define F_CPU 16000000UL
#include "twi_pca.h"
#include "lcd_expander.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
int main(){
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output (to send stuff to LEDs_
	PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); //Set EXT_PORT1[7:4] as input and [3:0] as output
    DDRB = 0xFF;                                //Set PORTB as output
    PORTB = 0x00;
    uint8_t reading;
    DDRD = 0xFF;
    PORTD = 0x00;
    
    while(1) {
        reading = keypad_to_ascii();
        if(reading == 'A') {
            PORTB = 0x01;
        }
        else if(reading == '8') {
            PORTB = 0x02;
        }
        else if(reading == '6') {
            PORTB = 0x04;
        }
        else if(reading == '*') {
            PORTB = 0x08;
        }
  
        else
        {
            PORTB = 0x00;
        }
    }
}