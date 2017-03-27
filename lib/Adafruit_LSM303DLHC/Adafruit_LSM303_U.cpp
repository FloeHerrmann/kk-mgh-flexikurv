#include "Arduino.h"
#include <TinyWireM.h>
#include <limits.h>

#include "Adafruit_LSM303_U.h"

static float _lsm303Accel_MG_LSB = 0.001F;   // 1, 2, 4 or 12 mg per lsb

void Adafruit_LSM303_Accel_Unified::write8(byte address, byte reg, byte value) {
	TinyWireM.beginTransmission(address);
	TinyWireM.write((uint8_t)reg);
	TinyWireM.write((uint8_t)value);
	TinyWireM.endTransmission();
}

byte Adafruit_LSM303_Accel_Unified::read8(byte address, byte reg) {
	byte value;

	TinyWireM.beginTransmission(address);
	TinyWireM.write((uint8_t)reg);
	TinyWireM.endTransmission();
	TinyWireM.requestFrom(address, (byte)1);
	value = TinyWireM.read();
	TinyWireM.endTransmission();

	return value;
}

void Adafruit_LSM303_Accel_Unified::read() {
	TinyWireM.beginTransmission((byte)LSM303_ADDRESS_ACCEL);
	TinyWireM.write(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80);
	TinyWireM.endTransmission();
	TinyWireM.requestFrom((byte)LSM303_ADDRESS_ACCEL, (byte)6);

	while (TinyWireM.available() < 6);

	uint8_t xlo = TinyWireM.read();
	uint8_t xhi = TinyWireM.read();
	uint8_t ylo = TinyWireM.read();
	uint8_t yhi = TinyWireM.read();
	uint8_t zlo = TinyWireM.read();
	uint8_t zhi = TinyWireM.read();

	raw.x = (int16_t)(xlo | (xhi << 8)) >> 4;
	raw.y = (int16_t)(ylo | (yhi << 8)) >> 4;
	raw.z = (int16_t)(zlo | (zhi << 8)) >> 4;
}

Adafruit_LSM303_Accel_Unified::Adafruit_LSM303_Accel_Unified(int32_t sensorID) {
	_sensorID = sensorID;

	raw.x = 0;
	raw.y = 0;
	raw.z = 0;
}

bool Adafruit_LSM303_Accel_Unified::begin() {
	TinyWireM.begin();
	write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);
	uint8_t reg1_a = read8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A);
	if (reg1_a != 0x57) {
		return false;
	}
	return true;
}

bool Adafruit_LSM303_Accel_Unified::getEvent(sensors_event_t *event) {
	memset(event, 0, sizeof(sensors_event_t));

	read();

	event->version   = sizeof(sensors_event_t);
	event->sensor_id = _sensorID;
	event->type      = SENSOR_TYPE_ACCELEROMETER;
	event->timestamp = millis();
	event->acceleration.x = (float)raw.x * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
	event->acceleration.y = (float)raw.y * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
	event->acceleration.z = (float)raw.z * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;

	return true;
}

void Adafruit_LSM303_Accel_Unified::getSensor(sensor_t *sensor) {
	memset(sensor, 0, sizeof(sensor_t));

	strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name)- 1] = 0;
	sensor->version     = 1;
	sensor->sensor_id   = _sensorID;
	sensor->type        = SENSOR_TYPE_ACCELEROMETER;
	sensor->min_delay   = 0;
	sensor->max_value   = 0.0F;
	sensor->min_value   = 0.0F;
	sensor->resolution  = 0.0F;
}