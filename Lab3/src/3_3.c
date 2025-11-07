#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>
#include <stdbool.h>
#include "pwm.h"
#include "adc.h"


int main(){	
	int DC_VALUES[14];		//Create table for dc_values

	for (int i = 0; i < 13; i++){		//set up the DC_VALUE table
		DC_VALUES[i] = (8 + i*20);
	}
    adc_init(0,0,0);			//Initialize ADC
    pwm_init();
	
	DDRB = 0xFF;        //PORTB as output
    DDRC = 0x00;        //PORTC is left as input
	DDRD = 0x00;		//PORTD as input				
	int index = 7;			//Duty Cycle =  50%
    OCR1AL = DC_VALUES[index];
    unsigned char mode1 = 1;
    unsigned char mode2 = 1;
    bool flag = true;

	while(1){		
        // Set the desired mode
        mode1 = PIND & 0x40;
        mode2 = PIND & 0x80;
        if (mode1 ==0) {
            flag = true;
            OCR1AL = DC_VALUES[index];
        }
        if (mode2 ==0) {
            flag = false;            
        }
        if(flag) {
            _delay_ms(100);
            unsigned char pd1 = PIND & 0x02, pd2 = PIND & 0x04, check = PIND;
        
            //wait for the button to be un-pressed
            while (!(check & (1 << PD1)) | !(check & (1<<PD2))) {
                check = PIND;
            }

            
            if((pd1 ==0) && index < 12){ // with PD1 we increase
                index++;
                OCR1AL = DC_VALUES[index];
            }
            if((pd2 ==0) && index > 0){ // with PD2 we decrease
                index--;
                OCR1AL = DC_VALUES[index];
            }
        }
        else {
            OCR1AL = adc_right_read() >> 2; 
        }
    }
}