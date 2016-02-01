#include "Arduino.h"
//#include "UVMDisplay.h"
#include "UVMCalibration.h"
//#include "UVMComms.h"
#include "UVmeter.h"

UVMDisplay *display;
//UVMCalibration *calib;
//UVMComms comms(&calib);
UVmeter *uvm;


void setup(void) {

	Serial.begin(9600);
	uvm = new UVmeter();
	uvm->setup();
}

void loop(void) {
	uvm->loop();
}




