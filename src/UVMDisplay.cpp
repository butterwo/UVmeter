/*
 * UVMDisplay.cpp
 *
 *  Created on: 8 Jan 2016
 *      Author: andrew
 */

#include "UVMDisplay.h"

UVMDisplay::UVMDisplay() {
	// TODO Auto-generated constructor stub
	u8g = new U8GLIB_SSD1306_128X64(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
	u8g->setColorIndex(1);         // pixel on
	sensorValue = 0.0;
	batteryVoltage = 0.0;
	batteryError = false;
	calibrationError = false;
	overRangeError = false;
	flash = false;
}

UVMDisplay::~UVMDisplay() {
	// TODO Auto-generated destructor stub
}

void UVMDisplay::initialize() {
	batteryVoltage = 10.0;
	u8g->begin();
	u8g->setRot180();  //rotate display
	u8g->firstPage();
	do {
//		// graphic commands to redraw the complete screen should be placed here
//		//u8g->setFont(u8g_font_unifont);
//		draw_header();
	} while( u8g->nextPage());

}
void UVMDisplay::draw() {
	// picture loop
	u8g->firstPage();
	do {
		//Serial.println(sensorValue);
		// graphic commands to redraw the complete screen should be placed here
		//u8g->setFont(u8g_font_unifont);
		draw_header();
		u8g->setFont(u8g_font_fub42n);
		if (batteryError || calibrationError) strcpy(displayValueStr, "");
		else if (overRangeError && flash) {
			strcpy(displayValueStr, "");
		}
		else {
			dtostrf(sensorValue, 6, 1, displayValueStr);
		}
		u8g->setFont(u8g_font_fur30n);
		u8g->drawStr(128-u8g->getStrWidth(displayValueStr+4), 64, displayValueStr+4);
		displayValueStr[4] = '\0';
		u8g->setFont(u8g_font_fur42n);
		u8g->drawStr(96-u8g->getStrWidth(displayValueStr), 64, displayValueStr);
		//Serial.println(displayValueStr);
		u8g->setFont(u8g_font_6x10r);
	} while( u8g->nextPage());
	flash = !flash;
}

void UVMDisplay::draw_header() {
	const char calibErrStr[] = "Calibration error";
	const char overRangeErrStr[] = "Over range";
	const char battErrStr[] = "Battery error";
	const char titleStr[] = "UV-Ery  mMED/h";
	if (batteryVoltage < battery_thresh[3]) batteryError = true;
	else batteryError = false;
	if (calibrationError) u8g->drawStr(128-u8g->getStrWidth(calibErrStr), 9, calibErrStr);
	else if (overRangeError) u8g->drawStr(128-u8g->getStrWidth(overRangeErrStr), 9, overRangeErrStr);
	else if (batteryError) u8g->drawStr(128-u8g->getStrWidth(battErrStr), 9, battErrStr);
	else u8g->drawStr(128-u8g->getStrWidth(titleStr), 9, titleStr);
	drawBatteryIndicator();
}

void UVMDisplay::drawBatteryIndicator() {
	const int x = 0;
	const int y = 0;
	u8g->drawFrame(x, y, 21, 10);
	u8g->drawBox(x+21, y+3, 2, 5);
	if (batteryVoltage > battery_thresh[2]) u8g->drawBox(x+2, y+2, 5, 6);
	if (batteryVoltage > battery_thresh[1]) u8g->drawBox(x+8, y+2, 5, 6);
	if (batteryVoltage > battery_thresh[0]) u8g->drawBox(x+14, y+2, 5, 6);
}

