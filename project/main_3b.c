/*
 * ECE 153B - Summer 2020
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"
#include "lcd.h"

int tmpCheckTransition = 0;
int savedCurrent = 0;

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
	// [TODO]
	
	// 1
	// enable clock for GPIO port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// Configure PB6
	// first reset the values to 00
	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	// next set GPIOE pin 8 as 10 such that 
	// it is in alternative	10
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	
	// set up AF2 for alternative function TIM4_CH1
	// 0010 for AF2
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	
	// configure PB6 as no pull-up no pull-down
	// reset the bits first for p2 					00
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
	
	// 2
	// enable timer4
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	// 3
	// set prescaler value to 15
	TIM4->PSC &= ~TIM_PSC_PSC;
	TIM4->PSC = 0XF;
	
	// 4 
	// enable auto reload preload in CR
	// set auto reload value to max
	TIM4->CR1 |= TIM_CR1_ARPE;
	
	// set the ARR value to max
	// 
	TIM4->ARR &= ~TIM_ARR_ARR;
	TIM4->ARR |= 0xFFFF;
	
	// 5
	// in ccmr, set bits to 01 for timer input 1 config
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	// 6 
	// enable capturing
	TIM4->CCER |= TIM_CCER_CC1E;
	// allow capture both rising and falling
	// CC1NP is first bit, CC1P is 0 bit
	// thus set to 11 with below
	TIM4->CCER |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);
	// TIM4->CCER |= TIM_CCER_CC1NP;
	
	// 7
	// In the DMA/Interrupt enable register, 
	// enable both interrupt and DMA requests. In
	// addition, enable the update interrupt.
	
	// enable interrupt
	TIM4->DIER |= TIM_DIER_CC1IE;
	
	// enable DMA
	TIM4->DIER |= TIM_DIER_CC1DE;
	
	// enable update interrupt
	TIM4->DIER |= TIM_DIER_UIE;
	
	// 8
	// enable update generation in event generation register
	TIM4->EGR |= TIM_EGR_UG;
	
	// 9 
	// clear update interrupt flag
	TIM4->SR &= ~TIM_SR_UIF;
	
	// 10
	// set direction of counter
	// enable counter in CR
	
	// set timer to upcounting direction
	// since upcounting, counter overflow will occur
	TIM4->CR1 &= ~TIM_CR1_DIR;
	
	// enable the counter
	TIM4->CR1 |= TIM_CR1_CEN;
	
	// 11
	// enable interrupt in NVIC and
	// set priority to 2
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
}

void TIM4_IRQHandler(void) {
	// [TODO]
	// overflow
	// formula in lecture
	// will need to resolve for overflow
	// only increment if tmpCheckTranstion == 1
	// and overflow occured
	if((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF){
		if(tmpCheckTransition == 1){
			overflowCount++;
		}
		
		TIM4->SR &= ~TIM_SR_UIF;
		
	}
	if((TIM4->SR & TIM_SR_CC1IF) == TIM_SR_CC1IF){
		if(tmpCheckTransition == 0){
			tmpCheckTransition = 1;
			currentValue = TIM4->CCR1;
		}
		else if(tmpCheckTransition == 1){
			tmpCheckTransition = 0;
			lastValue = currentValue;
			currentValue = TIM4->CCR1;
			
			if(overflowCount == 0){
				// if overflow didnt occur
				timeInterval = (currentValue - lastValue);
				overflowCount = 0;
			}
			else{
				// the input frequency is 1MHz
				// thus CCP is 1us
				timeInterval = (currentValue - lastValue + (0xFFFF)*overflowCount);
				overflowCount = 0;
			}
		}
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
}

void Trigger_Setup() {
	// [TODO]
	
	// 1
	// enable clock for GPIO port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	// Configure PE11
	// first reset the values to 00
	GPIOE->MODER &= ~GPIO_MODER_MODE11;
	// next set GPIOE pin 8 as 10 such that 
	// it is in alternative	10
	GPIOE->MODER |= GPIO_MODER_MODE11_1;
	
	// set up AF2 for alternative function TIM1_CH2
	// 0001 for AF1
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL11;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL11_0;
	
	// configure PE11 as no pull-up no pull-down
	// reset the bits first for p2 					00
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD11;
	
	// configure PE11 output type as push-pull
	// since push-pull is the intended reset, can keep as this
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT11;/
	
	// PE11 to very high output speed
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED11;
	
	// 2
	// Enable TIM1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	// 3 
	// set the prescaler value
	TIM1->PSC |= 0XF;
	
	// 4
	// possibly change control register
	// enable auto reload preload
	TIM1->CR1 |= TIM_CR1_ARPE;

	// set the ARR value to MAX
	TIM1->ARR &= ~TIM_ARR_ARR;
	TIM1->ARR |= 0xFFFF;
	
	// 5
	// want to set CCR 2
	//
	// set CCR value for trigger
	// used equations: duty cycle = (Ton = 10us)/(Ts = 0.65s) * 100
	// then set this val equal to duty cycle = CCR / (ARR + 1)
	// value ends up being 98.46 => 99
	//TIM1->CCR2 |= 0x63;
	
	// maybe clear CCR2
	// instead do 10
	TIM1->CCR2 |= 0xA;
	
	// 6
	// configure channel for output compare
	// clear bits
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	
	// set bits to output compare mode 0110
	TIM1->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	
	// use channel 2
	// enable output preload
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	// 7
	// enable output in CCMR
	TIM1->CCER |= TIM_CCER_CC2E;
	TIM1->CCER &= ~TIM_CCER_CC2P;
	
	// 8
	// enable off-state selection for run mode
	TIM1->BDTR |= TIM_BDTR_OSSR;
	
	// enable main output enable
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	// 9
	// enable update generation in event generation register
	TIM1->EGR |= TIM_EGR_UG;
	
	// 10
	// enable update interrupt and clear update interrupt flag
	TIM1->DIER |= TIM_DIER_UIE;
	// enable cc2ie
	
	// clear the interrupt flag
	TIM1->SR &= ~TIM_SR_UIF;
	
	// 11
	// Set the direction of the counter and 
	// enable the counter in the control register.
	
	// set timer to upcounting direction
	TIM1->CR1 &= ~TIM_CR1_DIR;
	
	// enable the counter
	TIM1->CR1 |= TIM_CR1_CEN;
	
	// 11
	// enable interrupt in NVIC and
	// set priority to 2
	//NVIC_EnableIRQ(TIM4_IRQn);
	//NVIC_SetPriority(TIM4_IRQn, 2);
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	// Setup LCD
	LCD_Initialization();
	LCD_Clear();
	
	char message[6];
	while(1) {
		// [TODO]
		// uint32_t distance = timeInterval / 58;
		// 150us - 25ms is the range
		// 1in - 200in approx
		
		// set comparison for 10,000 but can be modified
		if((int)(timeInterval / 58) > 0x2710){
			sprintf(message, "------");

		}
		else{
			sprintf((char*)message, "%6d",
				(int)(timeInterval / 58));
		}
		LCD_DisplayString((uint8_t *) message);
	}
}
