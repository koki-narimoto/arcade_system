/*
 * ECE 153B - Winter 2021
 *
 * Name(s):
 * Section:
 * Lab: 1A
 */

#include "stm32l476xx.h"
 
void Init() {
	// Enable HSI
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// Select HSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while((RCC->CFGR & RCC_CFGR_SWS) == 0);
	
	
	// Enable GPIO Clocks
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	
	// Initialize Red LED
	
	// Configure PB2 as output
	// first reset the values to 00
	GPIOB->MODER &= ~GPIO_MODER_MODE2;
	// next set GPIOB pin 2 as 01 such that it is in output
	GPIOB->MODER |= GPIO_MODER_MODE2_0;
	
	// configure PB2 output type as push-pull
	// since push-pull is the intended reset, can keep as this
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT2;
	
	// configure PB2 as no pull-up no pull-down
	// reset the bits first for p2 					00
	GPIOB->PUPDR &= GPIO_PUPDR_PUPD2;	
	
	// Initialize Green LED
	
	// Configure PE8 as output
	// first reset the values to 00
	GPIOE->MODER &= ~GPIO_MODER_MODE8;
	// next set GPIOE pin 8 as 01 such that 
	// it is in output														01
	GPIOE->MODER |= GPIO_MODER_MODE8_0;
	
	// configure PE8 output type as push-pull
	// since push-pull is the intended reset, can keep as this
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT8;
	
	// configure PE8 as no pull-up no pull-down
	// reset the bits first for p2 					00
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD8;	
	
	// Initialize Joystick
	
	// PA0 Center Button
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE0;
	// configure PA0 as pull-down
	// reset the bits first for p0 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
	// now will set the PA0 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_1;
	
	
	// PA1 Left Button
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	// configure PA1 as pull-down
	// reset the bits first for p1 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	// now will set the PA1 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_1;
	
	
	// PA2 Right Button
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE2;
	// configure PA2 as pull-down
	// reset the bits first for p2 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;
	// now will set the PA2 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_1;
	
	
	// PA3 Up Button
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE3;
	// configure PA3 as pull-down
	// reset the bits first for p3 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3;
	// now will set the PA3 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_1;


	// PA5 Down Button
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE5;
	// configure PA5 as pull-down
	// reset the bits first for p5 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	// now will set the PA3 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_1;
	
}
 
void pollCheck(){
	while(1){
		// checks if PA0 was pressed
		// GPIO_IDR_ID0
		uint32_t inputPin0 = (GPIOA->IDR & GPIO_IDR_ID0) == GPIO_IDR_ID0;
		
		// checks if PA1 was pressed
		// GPIO_IDR_ID1
		uint32_t inputPin1 = (GPIOA->IDR & GPIO_IDR_ID1) == GPIO_IDR_ID1;
		
		// checks if PA2 was pressed
		// GPIO_IDR_ID2
		uint32_t inputPin2 = (GPIOA->IDR & GPIO_IDR_ID2) == GPIO_IDR_ID2;
		
		// checks if PA3 was pressed
		// GPIO_IDR_ID3
		uint32_t inputPin3 = (GPIOA->IDR & GPIO_IDR_ID3) == GPIO_IDR_ID3;
		
		// checks if PA5 was pressed
		// GPIO_IDR_ID5
		uint32_t inputPin5 = (GPIOA->IDR & GPIO_IDR_ID5) == GPIO_IDR_ID5;
		
		// integer used to check if the button was pressed previously
		// this is also used to see if the button press is due to a button hold
		// or an actual press
		// 0 for not pressed and 1 for pressed
		int PA0_pressed = 0;
		int PA1_pressed = 0;
		int PA2_pressed = 0;
		int PA3_pressed = 0;
		int PA5_pressed = 0;
		
		
		// generally would want to check if it equals the id first
		if(inputPin0 != 0){
			// the center button was pressed
			// toggle both green and red LED PE8 and PB2 as output bit == 1
			// and reset to output bit == 0
			if(PA0_pressed == 0){
				PA0_pressed = 1;
				GPIOE->ODR ^= GPIO_ODR_OD8;
				GPIOB->ODR ^= GPIO_ODR_OD2;
			}
			
		}else if(inputPin1 != 0){
			// the left button was pressed
			// toggle green LED PE8 as output bit == 1 and output bit == 0
			
			if(PA1_pressed == 0){
				PA1_pressed = 1;
				GPIOE->ODR ^= GPIO_ODR_OD8;
			}
			
		}else if(inputPin2 != 0){
			// the right button was pressed
			// toggle red LED PB2 as output bit == 1 and output bit == 0
			
			if(PA2_pressed == 0){
				PA2_pressed = 1;
				GPIOB->ODR ^= GPIO_ODR_OD2;
			}
			
		}else if(inputPin3 != 0){
			// the up button was pressed
			// set both green and red LED PE8 and PB2 as output bit == 1
			
			if(PA3_pressed == 0){
				PA3_pressed = 1;
				GPIOE->ODR |= GPIO_ODR_OD8;
				GPIOB->ODR |= GPIO_ODR_OD2;
			}
			
		}else if(inputPin5 != 0){
			// the down button was pressed
			// reset both green and red LED PE8 and PB2 as output bit == 0
			
			if(PA5_pressed == 0){
				PA5_pressed = 1;
				GPIOE->ODR &= ~GPIO_ODR_OD8;
				GPIOB->ODR &= ~GPIO_ODR_OD2;
			}
		} else{
			if(PA0_pressed == 1){
				PA0_pressed = 0;
			}else if(PA1_pressed == 1){
				PA1_pressed = 0;
			} else if(PA2_pressed == 1){
				PA2_pressed = 0;
			} else if(PA3_pressed == 1){
				PA3_pressed = 0;
			} else if(PA5_pressed == 1){
				PA5_pressed = 0;
			}
		}
	}
}

int main() {
	// Initialization
	Init();
	
	// Polling to Check for Button Presses
	pollCheck();
	
	return 0;
}
