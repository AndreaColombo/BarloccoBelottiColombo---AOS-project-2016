#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "miosix.h"
#include "adc.h"

#define BUFF 30000
#define NUM0 215
#define NUM1 205

using namespace std;
using namespace miosix;

typedef Gpio<GPIOA_BASE,0> recalibratingButton;
typedef Gpio<GPIOD_BASE,12> green;
typedef Gpio<GPIOD_BASE,13> orange;
typedef Gpio<GPIOD_BASE,14> blue;
typedef Gpio<GPIOD_BASE,15> red;

short buffer[BUFF];
short x = 370;
Adc adc;
Mutex mutex;
ConditionVariable adcReadCv, calibrationCv;


void blinkLed (void *argv){ //blinka i led a giro mentre ricalibra penso sia figo
	green::mode(Mode::OUTPUT);
	blue::mode(Mode::OUTPUT);
	red::mode(Mode::OUTPUT);
	orange::mode(Mode::OUTPUT);
	//int duration = 1000000; //per ora la hardcodo
	for(int i = 0;i<20;i++){
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
		x=0;
		int var =0;
			cout << "CALIBRANDOOOOOOOOOOOOOOOOH " <<endl;
		for (int i = 0; i <200; i++){
			adc.start();
			var += adc.read();
		}
		var = var/200 +50;
		x = (short) var;
		cout << "Nuovo threshold: " << x << endl;
		Thread::sleep(1);
		blinkThread->join();
	}
}

void adcRead(void* argv){
	short tmp;
	while(1){
		adc.start();
		tmp=adc.read();
		if(tmp>x){			Lock <Mutex> lck(mutex);
			adcReadCv.wait(lck);
			for(int i=0;i<BUFF;i++){
				adc.start();
				buffer[i] = adc.read();				
			}
			for(int i =0; i<BUFF; i++)
				cout << buffer[i] << endl;
			cout << "campionamento finito" << endl;
			char ch;
			cin >> ch;
		}
		
	}
}

int main(){
	/*Thread *adcReadThread;
	Thread *calibrationThread;
	
	
	calibrationThread=Thread::create(recalibration,2048,1,(void*)0,Thread::JOINABLE);
	adcReadThread=Thread::create(adcRead,2048,1,(void*)0,Thread::JOINABLE);*/
	int tmp;
	x=0;
	while(1){
		
		int var =0;
		for (int i = 0; i <200; i++){
			adc.start();
			var += adc.read();
			cout << "CALIBRANDOOOOOOOOOOOOOOOOH " <<endl;
		}
		var = var/200 +50;
		x = (short) var;
		cout << "Nuovo threshold: " << x << endl;
		while(recalibratingButton::value()==0){
		//adcReadCv.signal();
		adc.start();
		tmp=adc.read();
		if(tmp>x){
			//Lock <Mutex> lck(mutex);
			//adcReadCv.wait(lck);
			for(int i=0;i<BUFF;i++){
				adc.start();
				buffer[i] = adc.read();				
			}
			for(int i =0; i<BUFF; i++)
				cout << buffer[i] << endl;
			cout << "campionamento finito" << endl;
			char ch;
			cin >> ch;
			}
		}
		
		//calibrationCv.signal();
	}
	/*adcReadThread->join();
	calibrationThread->join();*/
}
