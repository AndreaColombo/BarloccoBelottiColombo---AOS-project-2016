
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
ConditionVariable adcReadCv, calibrationCv;
int buffer[300];

void blinkLed (void *argv){ //blinka i led a giro mentre ricalibra
	green::mode(Mode::OUTPUT);
	blue::mode(Mode::OUTPUT);
	red::mode(Mode::OUTPUT);
	orange::mode(Mode::OUTPUT);
	
	for(int i = 0;i<10;i++){
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
	Thread *blinkThread;
	while(1){
		Lock <Mutex> lck(mutex);
		calibrationCv.wait(lck);
		blinkThread=Thread::create(blinkLed,2048,1,(void*)0,Thread::JOINABLE);
		for (int i = 0; i <200;i++){
			adc.start();
			x += adc.read();
			cout << "Calibrando "<<endl;
		}
		x = x/200;
		cout << "Nuovo threshold: " << x << endl;
		blinkThread->join();
	}
}

void adcRead(void* argv){
	int val;
	Lock <Mutex> lck(mutex);
	while(1){
		for (int i = 0; i<300; i++){
			adcReadCv.wait(lck);
			adc.start();
			val = adc.read();
			buffer[i] = val;
			Thread::sleep(50);
		}
		
		for (int i = 0; i<300; i++)
			cout << buffer[i] << endl;
		
		char c;
		cin >> c;
	}
}

int main(){
	Thread *adcReadThread;
	Thread *calibrationThread;
	
	adcReadThread=Thread::create(adcRead,2048,1,(void*)0,Thread::JOINABLE);
	calibrationThread=Thread::create(recalibration,2048,1,(void*)0,Thread::JOINABLE);
	while(1){
		while(recalibratingButton::value()==0){
			Thread::sleep(10); //polling su button
			adcReadCv.signal();	
		}
		calibrationCv.signal();
	}
	adcReadThread->join();
	calibrationThread->join();
}