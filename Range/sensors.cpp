/*
 * sensors.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: pvloon
 */
///////////////////////////////////
// includes
#include "sensors.h"

///////////////////////////////////
// init DHT
dht11 DHT11;

byte switchValue[6][2]; // Current switch Status
byte currentStatus= 0; // Current Range Status
int calibrate_dhtvalue;
int sensorValue[7][2] = { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }; // Current readings   6 holds DHT11 Temp
unsigned long sensorAge[7] = {  0, 0, 0, 0, 0, 0, 0}; // Current readings   6 holds DHT11 Temp

// Local
static byte previousStatus = 0;
static byte sensorType[] = { SENSOR_THERMISTOR, SENSOR_THERMISTOR, SENSOR_THERMISTOR, SENSOR_THERMISTOR,
		SENSOR_THERMISTOR, SENSOR_THERMISTOR, SENSOR_DHT11 };
static float readDHT11(int dht11_pin);
static void detectState();
static int readSensor(int sensorid);
static long sensorCalibrate[7]; // Calibrate readings 6 holds DHT
static int calibrate_average;

int readSensor(int sensorid) {
	// read the analog in  fsvalue:
	byte error;
	int value;
	int tValue = 0;
	for (int samples = 0; samples < 1; samples++) {
		int retry = 5;
		do {
			error = false;
			if (sensorType[sensorid] == SENSOR_DHT11) {
				value = (int) readDHT11(DHT11_PIN);
			} else
				value = analogRead(sensorid);
			if (value == 1023 or value < 0) { // Open lead
				// TODO:: not working anymore open lead
				if (DEBUG_SENSORS) Serial.print("Error on Sensor: ");
				if (DEBUG_SENSORS) Serial.print(sensorid);
				if (DEBUG_SENSORS) Serial.print(" ");
				error = true;
				delay(50);
			} else {
				error = false;
				tValue += value;
				delay(10); // Wait a bit
			}
		} while (--retry > 0 and error);
	}
	if (error) {
		return -1;
	} else
		return (tValue / 1) + sensorCalibrate[sensorid];
}

void updateTempSensors() {

	timer.pulse(LED_PIN, HIGH, 100);

	// Store previous values
	for (byte sensorid = 0; sensorid <= 6; sensorid++)
		sensorValue[sensorid][1] = sensorValue[sensorid][0];

	// read sensors
	for (byte sensorid = 0; sensorid <= 6; sensorid++) {
		sensorValue[sensorid][0] = readSensor(sensorid);
		sensorAge[sensorid] = millis();
		if (sensorValue[sensorid][0] < 0) showStatus(ERROR_READ_SENSOR);
		if (sensorType[sensorid] != SENSOR_DHT11) {
			sensorValue[sensorid][0] = constrain(sensorValue[sensorid][0],0,calibrate_average);
			sensorValue[sensorid][0] = calibrate_average - sensorValue[sensorid][0];
		}
	}
	detectState();
	if (currentStatus != previousStatus) {
		payLoad_Simple command;
		command.command = (currentStatus>0) ? COMMAND_ON : COMMAND_OFF;
		command.data = currentStatus;
//		if (sendMessageRF(RF_BASE_STATION, command)) previousStatus = currentStatus; 	// Updatestatus if send succesfull else try again
	}
}

void detectState() {

	byte sensorid;

	for (sensorid = 0; sensorid <= 5; sensorid++) {
		if (DEBUG_SENSORS) Serial.print("T");
		if (DEBUG_SENSORS) Serial.print(sensorid);
		if (DEBUG_SENSORS) Serial.print(" Value ");
		if (DEBUG_SENSORS) Serial.print(sensorValue[sensorid][0]);
		if (abs(sensorValue[sensorid][0]) < SENSOR_THRESHOLD) { // OFF
			if (DEBUG_SENSORS) Serial.print(" off");
			currentStatus &= ~(1 << sensorid);
		} else if (abs (sensorValue[sensorid][0] - sensorValue[sensorid][1]) < SENSOR_THRESHOLD) { // STEADY
			if (DEBUG_SENSORS) Serial.print(" ->");
			currentStatus |= (1 << sensorid);
		} else if (sensorValue[sensorid][0] - sensorValue[sensorid][1] > 0) { // UP
			if (DEBUG_SENSORS) Serial.print(" ^");
			currentStatus |= (1 << sensorid);
		} else { // DOWN
			if (DEBUG_SENSORS) Serial.print(" v");
			currentStatus |= (1 << sensorid);
		}
		if (DEBUG_SENSORS) Serial.print(" | ");
	}
	if (DEBUG_SENSORS) Serial.print(" T6");
	if (DEBUG_SENSORS) Serial.print(" Value ");
	if (DEBUG_SENSORS) Serial.print(sensorValue[6][0]);
	if (DEBUG_SENSORS) Serial.print(" T6");
	if (DEBUG_SENSORS) Serial.print(" Cal ");
	if (DEBUG_SENSORS) Serial.print(calibrate_dhtvalue);
	if (DEBUG_SENSORS) Serial.println();

	for (byte switchid = 0; switchid <= 5; switchid++) {
		if (DEBUG_SENSORS) Serial.print(" S");
		if (DEBUG_SENSORS) Serial.print(switchid);
		if (DEBUG_SENSORS)
			(switchValue[switchid][0]>0) ? Serial.print(" On ") : Serial.print(" Off ");
		switchValue[switchid][0] = !digitalRead(switchid + SWITCH_0);
	}
	if (DEBUG_SENSORS) Serial.println();


	return;
}

void updateSwitches() {

	// read sensors
	for (byte switchid = 0; switchid <= 5; switchid++) {
		switchValue[switchid][1] = switchValue[switchid][0];
	}
	for (byte switchid = 0; switchid <= 5; switchid++) {
		switchValue[switchid][0] = !digitalRead(switchid + SWITCH_0);
	}
}

void setupSwitches() {

	for (byte switchid = 0; switchid <= 5; switchid++) {
		pinMode(switchid + SWITCH_0, INPUT);
		digitalWrite(switchid + SWITCH_0, HIGH); // connect internal pull-up
	}

}


void calibratetimer() {
	calibrate(false);
}

void calibrate(bool forceCalibrate) {


	int tValue;
	byte error;
	int sensorValue[7];
	if (!forceCalibrate) if (currentStatus>0) return;

	showStatus(INFO_CALIBRATING);
	error = false;
	// Blank calibrate values
	for (byte sensorid = 0; sensorid <= 6; sensorid++)
		sensorCalibrate[sensorid] = 0;
	// Read raw sensorvalues
	for (byte sensorid = 0; sensorid <= 6; sensorid++) {
		sensorValue[sensorid] = readSensor(sensorid);
		if (sensorValue[sensorid] < 0) error = true;
	}
	tValue = 0;
	// Calculate average
	for (byte sensorid = 0; sensorid <= 5; sensorid++)
		tValue += sensorValue[sensorid];
	calibrate_average = tValue / 6;
	calibrate_dhtvalue = sensorValue[6];

	if (DEBUG_SENSORS) Serial.println();
	if (DEBUG_SENSORS) Serial.print("*Calibrate, tValue ");
	if (DEBUG_SENSORS) Serial.print(tValue);
	if (DEBUG_SENSORS) Serial.print(" |");
	if (DEBUG_SENSORS) Serial.print("avg ");
	if (DEBUG_SENSORS) Serial.print(calibrate_average);
	if (DEBUG_SENSORS) Serial.print(" |");
	if (DEBUG_SENSORS) Serial.print("DHT ");
	if (DEBUG_SENSORS) Serial.print(sensorValue[6]);
	if (DEBUG_SENSORS) Serial.println(" |");

	// Calculate Offset
	// Calculate calibration values
	if (error) goto errorHandler;
	for (int sensorid = 0; sensorid <= 5; sensorid++) {
		sensorCalibrate[sensorid] = (sensorValue[sensorid] - calibrate_average) * -1;

		if (DEBUG_SENSORS) Serial.print("SensorVal: ");
		if (DEBUG_SENSORS) Serial.print(sensorValue[sensorid]);
		if (DEBUG_SENSORS) Serial.print(" | ");
		if (DEBUG_SENSORS) Serial.print("CalibrateVal ");
		if (DEBUG_SENSORS) Serial.print(sensorCalibrate[sensorid]);
		if (DEBUG_SENSORS) Serial.println(" |");
	}
	if (DEBUG_SENSORS) Serial.print("SensorVal DHT11: ");
	if (DEBUG_SENSORS) Serial.print(sensorValue[6]);
	if (DEBUG_SENSORS) Serial.println(" |");

//good_exit:
	payLoad_Simple scommand;
	scommand.command = 	COMMAND_CALIBRATED;
	scommand.data = 0;
//	sendMessageRF(RF_BASE_STATION, scommand);
	return;

	errorHandler: for (int sensorid = 0; sensorid <= 5; sensorid++) {

		if (DEBUG_SENSORS) Serial.print("*Calibrate Error, SensorVal: ");
		if (DEBUG_SENSORS) Serial.print(sensorValue[sensorid]);
		if (DEBUG_SENSORS) Serial.print(" | ");
		if (DEBUG_SENSORS) Serial.print("CalibrateVal ");
		if (DEBUG_SENSORS) Serial.print(sensorCalibrate[sensorid]);
		if (DEBUG_SENSORS) Serial.println(" |");
	}
	if (DEBUG_SENSORS) Serial.print("SensorVal DHT11: ");
	if (DEBUG_SENSORS) Serial.print(sensorValue[6]);
	if (DEBUG_SENSORS) Serial.println(" |");
	showStatus(ERROR_CALIBRATE);
	return;
}

float readDHT11(int dht11_pin) {

	int chk = DHT11.read(dht11_pin);

	switch (chk) {
	case DHTLIB_OK:
		break;
	case DHTLIB_ERROR_CHECKSUM:
		return -1;
		break;
	case DHTLIB_ERROR_TIMEOUT:
		return -2;
		break;
	default:
		return -3;
		break;
	}

	return (float) DHT11.temperature;
	return -1;
}

