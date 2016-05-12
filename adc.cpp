#include "adc.h"
#include "miosix.h"
//we will be using only ADC1

using namespace miosix;

typedef Gpio<GPIOC_BASE,1> adcIn;

Adc::Adc(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= 1<<8;
	
	ADC1->CR1 = (1<<3) | (1<<1) | (1<<0) | (1<<8) |(1<<24)|(1<<25);
	
	ADC1->CR2 |= ADC_CR2_ADON;
	//ADC1->CR2 |= 1<<1;
	//GPIOC->MODER |= 1<<2;
	
	adcIn::mode(Mode::INPUT_ANALOG);
	
	ADC1->SQR1 |= (1<<23) |(1<<22) | (1<<21) | (1<<20);
	ADC1->SQR3 |= (1<<9);
	ADC1->SQR3 |= (1<<3) | (1<<1) | (1<<0);
}

void Adc::start(){
	//cr1 bit 8 scan
	//cr2 bit 30 start
	ADC1->CR1 |= 1<<8;

}

short Adc::read(){
	
	/* Wait till done */
	//while (!(ADC1->SR & ADC_SR_EOC)) {
		
	//}
	
	/* Return result */
	return ADC1->DR;
}