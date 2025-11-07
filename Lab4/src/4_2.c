#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>
#include "adc.h"
#include "lcd.h"
#include "pwm.h"
//uint8_t adc_low_val= 0;


int main() {
    // PORTD as output (to send data and commands to LCD)
    DDRD = 0xFF;
    //PORTC as input (for the correct operation of ADC)
    DDRC = 0x00;
    
    adc_init(1,0,0);
    lcd_init();
    _delay_ms(100);
    

    
    while(1) {
        _delay_ms(1000);
        uint16_t adc_value =0;
        uint8_t first_digit=0, second_digit=0, third_digit =0;
        //int dummy =0;
        lcd_clear_display();
        // Take ADC measurement
        adc_value = (adc_right_read()*5);
        
        // Find first digit
        first_digit = (adc_value / 1024);
        
        // Find fist digit after the decimal point (aka second digit)
        adc_value = adc_value % 1024;
        adc_value = adc_value*10;
        second_digit =  adc_value / 1024;
        
        // Find second digit after the decimal point (aka third digit)
        adc_value = adc_value % 1024;
        adc_value = adc_value*10;
        third_digit = adc_value / 1024;
        
        // Print in LCD screen
        lcd_data(first_digit + '0');
        lcd_data('.');
        lcd_data(second_digit+ '0');
        lcd_data(third_digit + '0');    
    }
    
    return 0;
}