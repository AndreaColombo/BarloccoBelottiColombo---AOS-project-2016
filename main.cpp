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

int x;
typedef Gpio<GPIOA_BASE,0> recalibratingButton;
typedef Gpio<GPIOD_BASE,12> green;
typedef Gpio<GPIOD_BASE,13> orange;
typedef Gpio<GPIOD_BASE,14> blue;
typedef Gpio<GPIOD_BASE,15> red;

Adc adc;
short buffer[BUFF];

int main(){
	short tmp;
	int count1=0,count0=0,bit;
	char code[100];
	for(int k=0;k<100;k++)
		code[k]='2';
	while(1){
		adc.start();
		tmp=adc.read();
		//cout << tmp << endl;
		if(tmp>380){
			for(int i=0;i<BUFF;i++){
				adc.start();
				buffer[i]=adc.read();
			}
			for(int i=0,k=0;i<BUFF;i++){
				if(buffer[i]>380){
					count1++;
					if(count0>0){
						cout << count0 << " blocco di 0" << endl;
						bit=(count0/NUM0)-1;
						for(int j=0;j<bit;j++){
							code[k]='0';
							k++;
						}
						count0=0;
					}
				}
				else{
					count0++;
					if(count1>0){
						cout << count1 << " blocco di 1" << endl;
						bit=(count1/NUM1);
						for(int j=0;j<bit;j++){
							code[k]='1';
							k++;
						}
						count1=0;
					}
				}
			}
			for(int k=0;code[k]!='2';k++)
				cout << code[k] << endl;
		}
	}
}