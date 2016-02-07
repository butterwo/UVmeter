/*
 * Thermo.cpp
 *
 *  Created on: 31 Jan 2016
 *      Author: andrew
 */

#include "DallasThermo.h"

DallasThermo::DallasThermo() {
	ONE_WIRE_BUS = 2;
	// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	oneWire = new OneWire(ONE_WIRE_BUS);

	// Pass our oneWire reference to Dallas Temperature.
	sensors = new DallasTemperature(oneWire);

}

DallasThermo::~DallasThermo() {
	// TODO Auto-generated destructor stub
}

/*
 * Setup function. Here we do the basics
 */
void DallasThermo::setup(void)
{
  sensors->begin();
  Serial.print("Found ");
  Serial.print(sensors->getDeviceCount(), DEC);
  Serial.println(" devices.");
  oneWire->reset_search();
  // assigns the first address found to myThermo
  if (!oneWire->search(myThermo)) Serial.println("Unable to find address for myThermo");

  // show the addresses we found on the bus
//  Serial.print("DallasThermo Device 0 Address: ");
//  printAddress(myThermo);
//  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors->setResolution(myThermo, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors->getResolution(myThermo), DEC);
  Serial.println();
}

// function to print the temperature for a device
float DallasThermo::getTemperature()
{
  float tempC = sensors->getTempC(myThermo);
  return tempC;
}

void DallasThermo::requestTemperature(void) {
	sensors->requestTemperatures(); // Send the command to get temperatures
}

/*
 * Main function. It will request the tempC from the sensors and display on Serial.
 */
//void DallasThermo::loop(void)
//{
//  // call sensors->requestTemperatures() to issue a global temperature
//  // request to all devices on the bus
//  Serial.print("Requesting temperatures...");
//  sensors->requestTemperatures(); // Send the command to get temperatures
//  Serial.print("DONE  ");
//
//  // It responds almost immediately. Let's print out the data
//  printTemperature(myThermo); // Use a simple function to print out the data
//}

// function to print a device address
void DallasThermo::printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
