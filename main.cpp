
#include <cstdio>
#include <iostream>
#include <string>
#include "miosix.h"
#include "adc.h"
using namespace std;
using namespace miosix;

int main(){
	Adc adc;
	int bla;
	string a;
	cout << "Prima del while" << endl;
	while(1){
		adc.start();
		cout << "conversione startata" << endl;
		bla = adc.read();
		cout << bla << endl;
	}
}
