
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
Mutex mutex;
ConditionVariable cv1,cv2;

void blinkLed (void *argv){ //blinka i led a giro mentre ricalibra penso sia figo
	green::mode(Mode::OUTPUT);
	blue::mode(Mode::OUTPUT);
	red::mode(Mode::OUTPUT);
	orange::mode(Mode::OUTPUT);
	//int duration = 1000000; //per ora la hardcodo
	for(;;){
		green::high();
		Thread::sleep(50);
		green::low();
		Thread::sleep(50);
		orange::high();
		Thread::sleep(50);
		orange::low();
		Thread::sleep(50);
		blue::high();
		Thread::sleep(50);
		blue::low();
		Thread::sleep(50);
		red::high();
		Thread::sleep(50);
		red::low();
		Thread::sleep(50);
	}
}


void recalibration (void* argv){
	while(1){
		recalibratingButton::mode(Mode::INPUT);
		while(recalibratingButton::value()==0)
			Thread::sleep(10); //polling su button
		Thread = *t;
		thread = Thread::create(blinkLed,2048,1,(void *)2,Thread::JOINABLE);
		Lock <Mutex> lock mutex;
		for (int i = 0; i<1000000; i++){
			//qua devo bloccare l'esecuzione del main finchè non si ricalibra
			x = (adc.read() + x)/i);
		}
		t->terminate();
	}
}


int main(){
	Thread *thread;
	thread = Thread::create(recalibration,2048,1,(void *)2,Thread::JOINABLE);
	cout << "Prima del while" << endl;
	while(1){
		adc.start();
		int bla = adc.read();
		if (bla > x)
			cout << "Valore letto: " << bla << endl;
		Thread::sleep(1000);
	}
	thread->join();
}