#include "adc.h"
#include "miosix.h"
//we will be using only ADC1

using namespace miosix;

typedef Gpio<GPIOC_BASE,1> adcIn;

Adc::Adc(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= 1<<8;
	
	ADC1->CR1 = 0; //resolution (11->6 bit)
	
	ADC1->CR2 |= ADC_CR2_ADON;
	//ADC1->CR2 |= 1<<1; quando starti la contnuos conversion mode
	//GPIOC->MODER |= 1<<2;
	ADC1->SMPR1 = (1<<3);
	adcIn::mode(Mode::INPUT_ANALOG);
	ADC->CCR |=(1<<17);
	ADC1->SQR1 =0;
	ADC1->SQR3 |= (1<<3) | (1<<1) | (1<<0);
}

void Adc::start(){
	//cr1 bit 8 scan
	//cr2 bit 30 start
	ADC1->CR2 |= 1<<30;
	while(!(ADC1->SR & (1<<4)));

}

short Adc::read(){
	
	/* Wait till done */
	while (!(ADC1->SR & (1<<1)));
	
	
	/* Return result */
	return ADC1->DR;
}