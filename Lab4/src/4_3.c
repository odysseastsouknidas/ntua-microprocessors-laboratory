#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>
#include <stdbool.h>
#include "pwm.h"
#include "adc.h"
#include "lcd.h"



int main() {
    // PORTD as output (to send data and commands to LCD)
    DDRD = 0xFF;
    //PORTC as input (for the correct operation of ADC)
    DDRC = 0x00;
    //PORTB as output for the LEDs
    DDRB = 0xFF;
    
    adc_init(2,0,0);
    lcd_init();
    
    _delay_ms(100);
    
    uint16_t adc_value =0;
    char clr[] = "CLEAR";
    char gas[] = "GAS DETECTED";
    bool already_on= 0, flag=0;
    int counter =0;
    
    while(1){
        _delay_ms(100);
        adc_value = adc_right_read();
        // 205 is the value for Vgas that gives density 70 ppm
        if(adc_value > 205) {  // density > 70 ppm
            flag=1;
            int dummy = adc_value / 16;
            if (already_on--) { // Turn LEDs off
                PORTB = 0x00;
                
            }
            else {              // Turn LEDs on
                PORTB = dummy;
                already_on++;
            }
            lcd_clear_display();
            for (int i=0; i<12; i++) {
                //int distance = gas[i] - 'A';
                lcd_data(gas[i]);
            }
            
        }
        else {  // density <= 70 ppm
            int dummy = adc_value / 16;
            PORTB = dummy;
            if(flag) {
                flag =0;
                lcd_clear_display();
                for (int i=0; i<5; i++) {
                    //int distance = clr[i] - 'A';
                    lcd_data(clr[i]);
                }
                counter++;
            }
            if(counter <= 10) {
                counter++;
            }
            else{
                lcd_clear_display();
                counter =0;
            }
        }
    }
    return 0;
}