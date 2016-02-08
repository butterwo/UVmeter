/*
 * UVmeter.cpp
 *
 *  Created on: 10 Jan 2016
 *      Author: andrew
 */

//#define TEST_ZERO_OFFSET

#include "UVmeter.h"
//#include UVMCalibration.h

UVmeter::UVmeter() {
	// TODO Auto-generated constructor stub
	calib = new UVMCalibration();
	display = new UVMDisplay();
	thermo = new DallasThermo();
	stringComplete = false;  // whether the string is complete
	inputString.reserve(80); // reserve 80 bytes for the inputString
	commandString.reserve(80); // reserve 80 bytes for the inputString
	commandString = "";         // a string to hold serial command
	meanSensorVal = 0;  // sensor ADC reading
	meanZeroVal = 0;  // sensor zero offset
	meanSensorHiVal = 0; // sensor high range ADC reading
	meanBatteryVal = 0;  // battery ADC reading
	// initialize serial:
	//Serial.begin(9600);
}

UVmeter::~UVmeter() {
	// TODO Auto-generated destructor stub
}

void UVmeter::handleSerialCommands() {
	// check for serial commands
	serialEvent();
	if (stringComplete) {
		commandString = inputString;
		//Serial.print("commandString: ");
		Serial.print(commandString);
		Serial.println("!");
		commandString.trim();
		//Serial.print("commandString: ");
		Serial.print(commandString);
		Serial.println("!");

		// clear the string:
		inputString = "";
		stringComplete = false;

		// execute command
		if (commandString.substring(0, 7) == "calib_1") {
			int ref = (commandString.substring(commandString.indexOf('(') + 1)).toInt();
			Serial.print("ref = ");
			Serial.println(ref);
			calib->writeSensorCalibPoint((float)ref, meanSensorVal, meanSensorHiVal);
		}
		else if (commandString == "calib_0") {
			Serial.println("Setting calibration offset...");
			calib->writeSensorZero(meanSensorVal, meanSensorHiVal);
		}
		else if (commandString == "calib_batt") {
			calib->writeBatteryCalib9V(meanBatteryVal);
		}
		else if (commandString == "calib_def") {
			calib->loadCalibrationData(0.0, 0.0, 26.0, 450.0, 108.0, 780.0);
		}
		else if (commandString.substring(0, 10) == "calib_load") {
			float values[6];
			String valueStr = commandString.substring(11);
			for (int i = 0; i < 6; i++) {
				int delim = valueStr.indexOf(',');
				values[i] = valueStr.substring(0, delim-1).toFloat();
				Serial.println(values[i]);
				valueStr = valueStr.substring(delim+1);
			}
			calib->loadCalibrationData(values[0], values[1], values[2], values[3], values[4], values[5]);
		}
		else if (commandString == "calib_print") {
			float zeroValue, zeroHiValue, ref, value, hiValue, battValue;
			calib->getCalibrationData(zeroValue, zeroHiValue, ref, value, hiValue, battValue);
			String outStr = String(zeroValue,2) + "," + String(zeroHiValue,2) + "," + String(ref,2) + "," + String(value,2) + "," + String(hiValue,2) + "," + String(battValue,2);
			Serial.println(outStr);
//			Serial.println(zeroValue);
//			Serial.println(zeroHiValue);
//			Serial.println(ref);
//			Serial.println(value);
//			Serial.println(hiValue);
//			Serial.println(battValue);
//			Serial.println();
		}
	}
}

/*
   SerialEvent occurs whenever a new data comes in the
   hardware serial RX.  This routine is run between each
   time loop() runs, so using delay inside loop can delay
   response.  Multiple bytes of data may be available.
 */
void UVmeter::serialEvent() {
	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') {
			stringComplete = true;
		}
	}
}

void UVmeter::setup(void) {

	// reset display
	// OLED is held in reset at power-up via a pull-down on RST pin (active low)
	pinMode(displayRstPin, OUTPUT);
	digitalWrite(displayRstPin, HIGH); // Drive reset false
	delay(50);
	display->initialize();
	//Serial.begin(9600);
	// Internal 1.1V ADC reference ?
	analogReference(EXTERNAL);
	// set up temperature sensor
	thermo->setup();
}

void UVmeter::loop(void) {
	//Serial.println("loop");
	//float meanSensorVal;
	//float meanSensorHiVal;
	float sdSensorVal;
	float sdZeroVal;
	float displayValue;
	//float meanBatteryVal;
	int16_t sensorVal;
	int32_t sumSensorVal = 0;
	int64_t sumSensorVal2 = 0.0;
#ifdef TEST_ZERO_OFFSET
	int16_t zeroVal;
	int32_t sumZeroVal = 0;
	int64_t sumZeroVal2 = 0.0;
#endif /* TEST_ZERO_OFFSET */
	int16_t sensorHiVal;
	int32_t sumSensorHiVal = 0;
	int16_t batteryVal;
	int32_t sumBatteryVal = 0.0;
	uint32_t nBatterySamples = 0;
	const uint16_t nSamples = 1000;

	// request a temperature measurement
	thermo->requestTemperature();

	for (uint16_t i = 0; i < nSamples; i++) {
		// read the value from the sensor:
		sensorVal = analogRead(sensorPin);
		sumSensorVal += sensorVal;
		sumSensorVal2 += (int32_t)sensorVal*sensorVal;
		// read the zero offset value
#ifdef TEST_ZERO_OFFSET
		zeroVal = analogRead(zeroRefPin);
		sumZeroVal += zeroVal;
		sumZeroVal2 += (int32_t)zeroVal*zeroVal;
#endif /* TEST_ZERO_OFFSET */
		// read the battery monitor value:
		sensorHiVal = analogRead(sensorHiPin);
		sumSensorHiVal += sensorHiVal;
		if (i%10 == 0) {
			batteryVal = analogRead(batteryPin);
			sumBatteryVal += batteryVal;
			nBatterySamples++;
		}
		//delayMicroseconds(100);
	}

	// get the temperature measurement we requested earlier
	float temperature = thermo->getTemperature();
	Serial.print("    Temperature: ");
	Serial.print(temperature);

	meanSensorVal = (float)sumSensorVal/nSamples;
	sdSensorVal = sqrt((float)(sumSensorVal2 - (int64_t)sumSensorVal*sumSensorVal/nSamples)/(nSamples - 1));
#ifdef TEST_ZERO_OFFSET
	meanZeroVal = (float)sumZeroVal/nSamples;
	sdZeroVal = sqrt((float)(sumZeroVal2 - (int64_t)sumZeroVal*sumZeroVal/nSamples)/(nSamples - 1));
#endif /* TEST_ZERO_OFFSET */
	meanSensorHiVal = (float)sumSensorHiVal/nSamples;
	meanBatteryVal = sumBatteryVal/nBatterySamples;

	Serial.print("    Sensor: ");
	Serial.print(meanSensorVal);
	Serial.print(" +/- ");
	Serial.print(sdSensorVal);
#ifdef TEST_ZERO_OFFSET
	Serial.print("    Zero: ");
	Serial.print(meanZeroVal);
	Serial.print(" +/- ");
	Serial.print(sdZeroVal);
	Serial.print("    Difference: ");
	Serial.print(meanSensorVal - meanZeroVal);
#endif /* TEST_ZERO_OFFSET */
	Serial.print("    Hi: ");
	Serial.println(meanSensorHiVal);

	// Update the display
	if (meanSensorVal < 1000) {
		displayValue = (meanSensorVal - calib->sensorOffset - 0.0128 * (temperature - 24.0)) * calib->sensorScaling;

		// TODO fix this properly!!
		displayValue += 0.12; // fiddle for error due to USB powering

		//Serial.println(calib->sensorOffset);
		//Serial.println(calib->sensorScaling);
	}
	else {
		displayValue = (meanSensorHiVal - calib->sensorHiOffset) * calib->sensorHiScaling;
	}
	if (displayValue > 799.9) {
		displayValue = 799.9;
		display->overRangeError = true;
	}
	else {
		display->overRangeError = false;
	}
	//if (displayValue < 0) displayValue = 0; // clip -ve values to 0
	//Serial.println(displayValue);
	display->sensorValue = displayValue;
	display->batteryVoltage = meanBatteryVal * calib->batteryScaling;
	display->calibrationError = calib->calibrationError;
	display->draw();

	// handle any serial commands which may arrive
	handleSerialCommands();
	//delay(50);
}

