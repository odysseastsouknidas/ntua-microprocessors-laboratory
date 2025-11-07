#define F_CPU 16000000UL
#include "therm.h"
#include "lcd_expander.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main() {
    //For the LCD Screen
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00);
    lcd_init();
    _delay_ms(100);             //Maybe for the LCD
    lcd_clear_display();
    
    DDRB = 0xFF;
    PORTB = 0x00;
    
    char problem[] = "NO Device";
    uint16_t reading;
    uint8_t int_part, dec_part;
    uint16_t previous_reading;
    while(1)
    {
        //Take reading
        reading = our_func();
        //If we have found no device
        if(reading == 0x8000)
        {
            lcd_clear_display();
            
            for(int i=0; i<9; i++)
            {
                lcd_data(problem[i]);
            }
            _delay_ms(100);
        }
        else
        {
            //If the reading hasn't changed go to the next iteration of the while loop
            //To avoid LCD flickering
            if(reading == previous_reading)
            {
                continue;
            }
            previous_reading = reading;
            lcd_clear_display();
            
            if((reading & 0x8000) == 1)     //Negative
            {
                reading = (~(reading)) + 1;
                lcd_data('-');
                int_part = integer_part(reading);
                dec_part = decimal_part(reading);
                send_three_to_screen(int_part);
                lcd_data('.');
                send_three_to_screen(dec_part);
                lcd_data(' ');
                lcd_data('o');
                lcd_data('C');
            }
            else                            //Positive
            {
                int_part = integer_part(reading);
                dec_part = decimal_part(reading);
                lcd_data('+');
                send_three_to_screen(int_part);
                lcd_data('.');
                send_three_to_screen(dec_part);
                lcd_data(' ');
                lcd_data('\xdf');
                lcd_data('C');
                
                //lcd_data(0x52);
            }
        }
    }
}