#include <cstdio>
#include <iostream>
#include <string>

#include "miosix.h"
#include "adc.h"

#define BUFF 30000

using namespace std;
using namespace miosix;

typedef Gpio<GPIOA_BASE,0> recalibratingButton;

Adc adc;


int main(){
	int tmp;
	short x=0;
	short buffer[BUFF];
	while(1){
		int var =0;
		for (int i = 0; i <200; i++){
			adc.start();
			var += adc.read();
			cout << "Calibrando " << i <<endl;
		}
		var = var/200 +50;
		x = (short) var;
		cout << "Nuovo threshold: " << x << endl;
		while(recalibratingButton::value()==0){
			adc.start();
			tmp=adc.read();
			if(tmp>x){
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
}
