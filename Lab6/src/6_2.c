#define F_CPU 16000000UL
#include "twi_pca.h"
#include "keypad.h"
#include "lcd_expander.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


int main() {
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //PORT0 as output (just to send stuff to LCD screen)
	PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); //PORT1[7:3] = input, [3:0] = output
    lcd_init();
    lcd_clear_display();
    uint8_t reading;
    char no_keys1[] = "PRESS SOME KEYS", no_keys2[] = "ALREADY";
    for(int i=0; i<strlen(no_keys1); i++)
    {
        lcd_data(no_keys1[i]);
    }
    lcd_change_line();
    for(int i=0; i<strlen(no_keys2); i++)
    {
        lcd_data(no_keys2[i]);
    }
    
    while(1)
    {
        reading = keypad_to_ascii();
        if(reading != 0)
        {
            lcd_clear_display();
            lcd_data(reading);
        }
    }
}