#define F_CPU 16000000UL
#include "twi_pca.h"
#include "lcd_expander.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main() {
    twi_init();
	PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); //PORT1[7:3] = input, [3:0] = output
    DDRB = 0xFF;                                //Set PORTB as output;
    PORTB = 0x00;
    uint8_t first, second;
    
    while(1) {
        
        //Maybe we need some delays between the whiles to avoid de-bouncing??
        
        //Wait for the first key to be pressed and when it is, store it in variable first
        while((first = keypad_to_ascii()) == 0);
        
        //Loop up until the user un-presses the first key
        while((keypad_to_ascii()) != 0);
        
        //Wait for the second key to be pressed and when it is, store it in variable second
        while((second = keypad_to_ascii()) == 0);
        
        //Now we have the values of the pressed keys and we can continue
        if(first == '5' && second == '8')
        {
            PORTB = 0xFF;
            _delay_ms(3000);
            PORTB = 0x00;
            _delay_ms(2000);
        }
        else
        {
            for(int i=0; i<5; i++)
            {
                PORTB = 0xFF;
                _delay_ms(500);
                PORTB = 0x00;
                _delay_ms(500);
            }
        }
    }
}
    
    
