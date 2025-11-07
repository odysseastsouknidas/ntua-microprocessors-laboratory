#define F_CPU 16000000UL
#include "pwm.h"
#include "adc.h"
#include "avr/io.h"
#include <util/delay.h>

int main(){
	
	adc_init(1,0,0);			//Initialize ADC
    pwm_init();
	
	int DC_VALUES[14];		//Create table for dc_values
	
	for (int i = 0; i < 13; i++){		//set up the DC_VALUE table
		DC_VALUES[i] = (8 + i*20);
	}
	
    DDRB = 0xFF;       //PB5-0 as output
    DDRC= 0x00;              //PORTC as input
	DDRD = 0b00011111;		//PD4-PD0 output the rest is input			
	int index = 7;			//Duty Cycle =  50%
    OCR1AL = DC_VALUES[index];
	
	uint16_t ADC_RESULT = 0;
	int reset = 0;
	
	while(1){	
		if(reset == 16){
			reset = 0;
			ADC_RESULT = (ADC_RESULT >> 4); //Divide by 2^4 = 16
            if (ADC_RESULT >= 0 & ADC_RESULT <= 200) {
                PORTD = 0x01;
            }
            else if(ADC_RESULT > 200 & ADC_RESULT <= 400) {
                PORTD = 0x02;
            }
            else if(ADC_RESULT > 400 & ADC_RESULT <= 600) {
                PORTD = 0x04;
            }
            else if(ADC_RESULT > 600 & ADC_RESULT <= 800) {
                PORTD = 0x08;
            }
            else {
                PORTD = 0x10;
            }
            ADC_RESULT =0;
		}
		_delay_ms(100);					//Call delay
		ADC_RESULT += adc_right_read();		//Read ADC_value
		reset++;						//Keep track of how many values we have
		
        /*Increase or decrease the Duty Cycle
		based on which button is pressed*/
        unsigned char d = PIND, check = PIND;
        
        //wait for the button to be un-pressed
        while (!(check & (1 << PD5)) | !(check & (1<<PD6))) {
            check = PIND;
        }
        
        unsigned char dummy5 = d &0x20;
        unsigned char dummy6 = d &0x40;

		if((dummy5 ==0) && index < 12){ // with PD5 we increase
			index++;
			OCR1AL = DC_VALUES[index];
		}
		if((dummy6 ==0) && index > 0){ // with PD6 we decrease
			index--;
			OCR1AL = DC_VALUES[index];
        }
														
		
	}
}
