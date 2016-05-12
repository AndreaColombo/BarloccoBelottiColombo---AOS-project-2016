
#include <cstdio>
#include <iostream>
#include <string>
#include "miosix.h"
#include "adc.h"
using namespace std;
using namespace miosix;

int main(){
	Adc adc;
	
	string a;
	cout << "Prima del while" << endl;
	cin >> a;
	while(1){
		adc.start();
		cout << "dopo start" << endl;
		int bla = adc.read();
		cout << bla << endl;
	}
}
