/*
 * UVMCalibration.h
 *
 *  Created on: 8 Jan 2016
 *      Author: andrew
 */

#ifndef SRC_UVMCALIBRATION_H_
#define SRC_UVMCALIBRATION_H_

#include "Arduino.h"
#include <EEPROM.h>

class UVMCalibration {
public:
	UVMCalibration();
	virtual ~UVMCalibration();
	void writeSensorZero(float, float);
	void writeSensorCalibPoint(float, float, float);
	void writeBatteryCalib9V(float);
	void loadCalibrationData(float, float, float, float, float, float);
	void getCalibrationData(float&, float&, float&, float&, float&, float&);
	void updateCRC();
	unsigned long calculateCRC();
	float sensorScaling;
	float sensorHiScaling;
	float batteryScaling;
	float sensorOffset;
	float sensorHiOffset;
	bool calibrationError;
private:
	// EEPROM addresses for calibration data
	const int eeAddressCalib0 = 0;
	const int eeAddressCalib0Hi = 4;
	const int eeAddressCalib1Ref = 8;
	const int eeAddressCalib1Val = 12;
	const int eeAddressCalib1HiVal = 16;
	const int eeAddressCalibBatt = 20;
	const int eeAddressCalibCRC = 24;
};

#endif /* SRC_UVMCALIBRATION_H_ */
