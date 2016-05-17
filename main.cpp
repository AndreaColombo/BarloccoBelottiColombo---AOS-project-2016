
#include <cstdio>
#include <iostream>
#include <string>

#include "miosix.h"
#include "adc.h"
using namespace std;
using namespace miosix;

int x;
typedef Gpio<GPIOA_BASE,0> recalibratingButton;
typedef Gpio<GPIOD_BASE,12> green;
typedef Gpio<GPIOD_BASE,13> orange;
typedef Gpio<GPIOD_BASE,14> blue;
typedef Gpio<GPIOD_BASE,15> red;
Adc adc;

void blinkLed (){ //blinka i led a giro mentre ricalibra penso sia figo
	green::mode(Mode::OUTPUT);
	blue::mode(Mode::OUTPUT);
	red::mode(Mode::OUTPUT);
	orange	::mode(Mode::OUTPUT);
	int duration = 1000000; //per ora la hardcodo
	for(int i = 0; i<1000000; i++){
		green::high();
		Thread::sleep(10);
		green::low();
		orange::high();
		Thread:sleep(10);
		orange::low();
		blue::high();
		Thread::sleep(10);
		blue::low();
		red::high();
		Thread::sleep(10);
		red::low();
	}
}


void recalibration (){
	recalibratingButton::mode(Mode::INPUT);
	while(recalibratingButton::value()==0)
		Thread::sleep(10);
	thread t2(blinkLed);
	for (int i = 0; i<1000000; i++){
		//qua devo bloccare l'esecuzione del main finchè non si ricalibra
		x = adc.read();
	}
	 
	  
	
	
}

int main(){
	thread t (recalibration);
	cout << "Prima del while" << endl;
	while(1){
		adc.start();
		int bla = adc.read();
		if (bla > x)
			cout << "Valore letto: " << bla << endl;
		Thread::sleep(1000);
	}
}