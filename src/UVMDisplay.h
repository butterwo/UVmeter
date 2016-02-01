/*
 * UVMDisplay.h
 *
 *  Created on: 8 Jan 2016
 *      Author: andrew
 */

#ifndef SRC_UVMDISPLAY_H_
#define SRC_UVMDISPLAY_H_

#include "Arduino.h"
#include "U8glib.h"

class UVMDisplay {
public:
	UVMDisplay();
	virtual ~UVMDisplay();
	void initialize();
	void draw();
	void draw_header();

	U8GLIB_SSD1306_128X64 *u8g;
	float sensorValue;
	float batteryVoltage;
	boolean batteryError;
	boolean calibrationError;
	boolean overRangeError;
	boolean flash;
	char displayValueStr[8];

private:
	void drawBatteryIndicator();
	const float battery_thresh[4] = {8.1, 7.7, 7.3, 6.9};
};

#endif /* SRC_UVMDISPLAY_H_ */
