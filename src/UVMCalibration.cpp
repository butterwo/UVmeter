/*
 * UVMCalibration.cpp
 *
 *  Created on: 8 Jan 2016
 *      Author: andrew
 */

#include "UVMCalibration.h"

UVMCalibration::UVMCalibration() {
	// TODO Auto-generated constructor stub
	sensorScaling = 0.058;
	sensorHiScaling = 0.24;
	sensorHiOffset = 0.0;
	sensorOffset = 0.0;
	batteryScaling = 0.02115;
	//calibrationError = true;
	calibrationError = true;
	//get calibration from EEPROM. Check CRC to make sure it's not corrupted.

	unsigned long crc;
	EEPROM.get(eeAddressCalibCRC, crc);
	Serial.print("crc: ");
	Serial.println(crc);
	Serial.print("calculateCRC: ");
	Serial.println(calculateCRC());
	if (calculateCRC() == crc) {
		float ref, value;
		EEPROM.get(eeAddressCalib0, sensorOffset);
		EEPROM.get(eeAddressCalib0Hi, sensorHiOffset);
		sensorScaling = (EEPROM.get(eeAddressCalib1Ref, ref)) / (EEPROM.get(eeAddressCalib1Val, value) - sensorOffset);
		sensorHiScaling = (EEPROM.get(eeAddressCalib1Ref, ref)) / (EEPROM.get(eeAddressCalib1HiVal, value) - sensorHiOffset);
		Serial.print("sensorScaling: ");
		Serial.println(sensorScaling, 6);
		batteryScaling = 9.0 / (EEPROM.get(eeAddressCalibBatt, value));
		Serial.print("batteryScaling: ");
		Serial.println(batteryScaling, 6);
		calibrationError = false;
	}

}

UVMCalibration::~UVMCalibration() {
	// TODO Auto-generated destructor stub
}

void UVMCalibration::writeSensorZero(float value, float hiValue) {
	Serial.print("0 mMED/h calibration value: ");
	Serial.println(value);
	Serial.print("0 mMED/h hi range calibration value: ");
	Serial.println(hiValue);
	EEPROM.put(eeAddressCalib0, value);
	EEPROM.put(eeAddressCalib0Hi, hiValue);
	updateCRC();
}

void UVMCalibration::writeSensorCalibPoint(float ref, float value, float hiValue) {
	Serial.print(ref);
	Serial.print(" mMED/h calibration value: ");
	Serial.println(value);
	Serial.print(ref);
	Serial.print(" mMED/h hi range calibration value: ");
	Serial.println(hiValue);
	EEPROM.put(eeAddressCalib1Ref, ref);
	EEPROM.put(eeAddressCalib1Val, value);
	EEPROM.put(eeAddressCalib1HiVal, hiValue);
	updateCRC();
}

void UVMCalibration::writeBatteryCalib9V(float value) {
	Serial.print(" Battery 9V calibration value: ");
	Serial.println(value);
	EEPROM.put(eeAddressCalibBatt, value);
	updateCRC();
}

void UVMCalibration::loadCalibrationData(float zeroValue, float zeroHiValue, float ref, float value, float hiValue, float battValue) {
	EEPROM.put(eeAddressCalib0, zeroValue);
	EEPROM.put(eeAddressCalib0Hi, zeroHiValue);
	EEPROM.put(eeAddressCalib1Ref, ref);
	EEPROM.put(eeAddressCalib1Val, value);
	EEPROM.put(eeAddressCalib1HiVal, hiValue);
	EEPROM.put(eeAddressCalibBatt, battValue);
	updateCRC();
}

void UVMCalibration::getCalibrationData(float &zeroValue, float &zeroHiValue, float &ref, float &value, float &hiValue, float &battValue) {
	EEPROM.get(eeAddressCalib0, zeroValue);
	EEPROM.get(eeAddressCalib0Hi, zeroHiValue);
	EEPROM.get(eeAddressCalib1Ref, ref);
	EEPROM.get(eeAddressCalib1Val, value);
	EEPROM.get(eeAddressCalib1HiVal, hiValue);
	EEPROM.get(eeAddressCalibBatt, battValue);
}

void UVMCalibration::updateCRC() {
	unsigned long crc = calculateCRC();
	EEPROM.put(eeAddressCalibCRC, crc);
}

/*
 * Calculate CRC of the calibration data
 */
unsigned long UVMCalibration::calculateCRC() {

	const unsigned long crc_table[16] = {
			0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
			0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
			0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
			0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};

	unsigned long crc = ~0L;

	for (int index = 0 ; index < 6  ; ++index) {
		crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
		crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
		crc = ~crc;
	}
	return crc;
}
