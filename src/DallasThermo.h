/*
 * Thermo.h
 *
 *  Created on: 31 Jan 2016
 *      Author: andrew
 */

#ifndef SRC_DALLASTHERMO_H_
#define SRC_DALLASTHERMO_H_

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino



class DallasThermo {
public:
	DallasThermo();
	virtual ~DallasThermo();
	void setup(void);
	void requestTemperature(void);
	float getTemperature(void);
	//void loop(void);
	void printAddress(DeviceAddress);
private:
	uint8_t ONE_WIRE_BUS;
	OneWire * oneWire;
	DallasTemperature * sensors;
	DeviceAddress myThermo;
};

#endif /* SRC_DALLASTHERMO_H_ */
