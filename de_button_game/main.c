/*
 * de_button_game.c
 *
 * Created: 24/03/2020 21:04:11
 * Author : SARM
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int Pressed_Confidence_level[2];
int Released_Confidence_level[2];
int Pressed[2]; 
// which LED is being led up
int LEDNumber[2];


void ProcessPressedButton(int ButtonPressed);
void ProcessReleasedButton(int ButtonPressed);

int main(void)
{
    /* Initialization area. Setting up ports and data direction registers */
	// control inputs  PC0, PC1
	// set DD to 0 for inputs
	DDRC &= ~(1 << DDC0);
	DDRC &= ~(1 << DDC1);
	// set for high reading
	PORTC |= (1 << DDC0);
	PORTC |= (1 << DDC1);
	// DD to 1 for output
	DDRB = -1;
	DDRD |= ~(3 << 6);
	 
    while (1)
	{
		if (bit_is_clear(PINC, 0) /*button pressed for side 1*/) {
			/* lighten up LED on portB on at a time */
			ProcessPressedButton(0);
		} else {
			/* Make sure button was released */
			ProcessReleasedButton(0);
		}
		if (bit_is_clear(PINC, 1)/*button pressed for side 2*/) {
			/* lighten up LED on portB on at a time */
			ProcessPressedButton(1);
		} else {
			/* Make sure button was released */
			ProcessReleasedButton(1);
		}
		// Process button clicks for side 1
		// Process button click for side 2
    }
}

void ProcessPressedButton(int ButtonPressed) {
	Pressed_Confidence_level[ButtonPressed]++;
	if (Pressed_Confidence_level[ButtonPressed] > 1)
	{
		if (Pressed[ButtonPressed] == 0)
		{
			Pressed[ButtonPressed] = 1;
			if (ButtonPressed == 0) PORTB |= 1 << LEDNumber[ButtonPressed];
			if (ButtonPressed == 1) PORTD |= 1 << LEDNumber[ButtonPressed];
			LEDNumber[ButtonPressed]++;
			if (LEDNumber[ButtonPressed] > 6)
			{
				// PORT? is winner so blink them
				for(int i=0; i < 10;i++){
					if (ButtonPressed == 0) PORTB &= ~(-1);
					if (ButtonPressed == 1) PORTD &= ~(-1);
					_delay_ms(30);
					if (ButtonPressed == 0) PORTB |= -1;
					if (ButtonPressed == 1) PORTD |= -1;
					_delay_ms(30); 
				}
				LEDNumber[0] = 0;
				LEDNumber[1] = 0;
			}
		}
		Pressed_Confidence_level[ButtonPressed] = 0;
	}
}

void ProcessReleasedButton(int ButtonPressed) {
	Released_Confidence_level[ButtonPressed]++;
	if (Released_Confidence_level[ButtonPressed] > 1)
	{
		Pressed[ButtonPressed] = 0;
		Released_Confidence_level[ButtonPressed] = 0;
	}
}