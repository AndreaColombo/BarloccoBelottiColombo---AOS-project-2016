
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
int buffer[300];

int main(){
	while(1){
		while(recalibratingButton::value()==0){
			for(int i=0;i<300;i++){
				adc.start();
				buffer[i]=adc.read();
			}
			for(int i=0;i<300;i++){
				cout << buffer[i] << endl;
			}
		}
	}
}