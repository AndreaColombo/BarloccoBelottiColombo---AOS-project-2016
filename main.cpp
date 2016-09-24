#include <cstdio>
#include <iostream>
#include <string>

#include "miosix.h"
#include "adc.h"
using namespace std;
using namespace miosix;

#define BUFF 30000
#define NUM0 215
#define NUM1 205
#define THRESHOLD 370

Adc adc;
short buffer[BUFF];

int main(){
	short tmp;
	
	while(1){
		adc.start();
		tmp=adc.read();
		//cout << tmp << endl;
		if(tmp>THRESHOLD){
			for(int i=0;i<BUFF;i++){
				adc.start();
				tmp = adc.read();
				if (tmp > THRESHOLD+50)
					buffer[i]=1;
				else buffer[i]=0;
			}
			for(int i =0; i<BUFF; i++)
				cout << buffer[i] << endl;
		}
	}
}