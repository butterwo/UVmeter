/*
 * UVmeter.h
 *
 *  Created on: 10 Jan 2016
 *      Author: andrew
 */

#ifndef SRC_UVMETER_H_
#define SRC_UVMETER_H_

#include "Arduino.h"
#include "UVMCalibration.h"
#include "UVMDisplay.h"

class UVmeter {
public:
	UVmeter();
	virtual ~UVmeter();
	void setup();
	void loop();
	void serialEvent();
	void handleSerialCommands();
	void executeCommand(char *commandString);
private:
	String inputString;         // a string to hold incoming data
	boolean stringComplete;  // whether the string is complete
	String commandString;         // a string to hold serial command
	UVMCalibration *calib;
	UVMDisplay *display;
	//UVMComms *comms(&calib);
	const int sensorPin = A0;    // the input pin for the UV sensor
	const int sensorHiPin = A1;   // the input pin for the UV sensor high range attenuated
	const int zeroRefPin = A2;   // the input pin for the zero offset reference
	const int batteryPin = A7;   // the input pin for the battery monitor
	const int displayRstPin =  7;      // the pin connected to OLED reset
	float meanSensorVal;  // sensor ADC reading
	float meanZeroVal;  // zero offset value
	float meanSensorHiVal; // sensor high range ADC reading
	float meanBatteryVal;  // battery ADC reading
};

#endif /* SRC_UVMETER_H_ */
