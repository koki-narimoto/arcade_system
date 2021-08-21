/*
 * ECE 153B - Summer 2020
 *
 * Name(s):
 * Section:
 * Lab: 1B
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize Joystick
	// Configure SYSCFG EXTI
	// [TODO]
	
	// Configure EXTI Trigger
	// [TODO]
	
	// Enable EXTI
	// [TODO]
	
	// Configure and Enable in NVIC
	// [TODO]
	
	
	//
	// PA0 Center Button
	//
	
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE0;
	// configure PA0 as pull-down
	// reset the bits first for p0 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
	// now will set the PA0 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_1;
	
	// PA0 for center button BOTH LED
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	// Falling Edge trigger for pin0
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;
	
	// Enable EXTI for input
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	
	// Configure enable and mask
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	
	// clear the pending bit
	EXTI->PR1 |= EXTI_PR1_PIF0;
	
	//
	// PA1 Left Button
	//
	
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	// configure PA1 as pull-down
	// reset the bits first for p1 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	// now will set the PA1 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_1;
	
	// PA1 for left button GREEN LED
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
	
	// Falling Edge trigger for pin2
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	
	// Enable EXTI for input
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	
	// Configure enable and mask
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 0);
	
	// clear the pending bit
	EXTI->PR1 |= EXTI_PR1_PIF1;
	
	//
	// PA2 Right Button
	//
	// first reset the values to 00 for input
	GPIOA->MODER &= ~GPIO_MODER_MODE2;
	// configure PA2 as pull-down
	// reset the bits first for p2 					00
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;
	// now will set the PA2 as pull-down 10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_1;
	
	// Configure the interrupt
	// PA2 for right button RED LED
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA;
	
	// Falling Edge trigger for pin2
	EXTI->FTSR1 |= EXTI_FTSR1_FT2;
	
	// Enable EXTI for input
	EXTI->IMR1 |= EXTI_IMR1_IM2;
	
	// Configure enable and mask
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI2_IRQn, 0);
	
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)

void EXTI0_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF0;
	
	// Define behavior that occurs when interrupt occurs
	// want to toggle both LED
	Red_LED_Toggle();
	Green_LED_Toggle();
}

void EXTI1_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF1;
	
	// Define behavior that occurs when interrupt occurs
	// want to toggle green
	Green_LED_Toggle();
}

void EXTI2_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF2;
	
	// Define behavior that occurs when interrupt occurs
	// want to toggle red
	Red_LED_Toggle();
}