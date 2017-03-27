#ifndef __L3GD20_H__
#define __L3GD20_H__

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <TinyWireM.h>

#define L3GD20_ADDRESS (0x6A)
#define L3GD20_POLL_TIMEOUT (100)
#define L3GD20_ID (0xD4)
#define L3GD20H_ID (0xD7)

// Sesitivity values from the mechanical characteristics in the datasheet.
#define GYRO_SENSITIVITY_250DPS  (0.00875F)
#define GYRO_SENSITIVITY_500DPS  (0.0175F)
#define GYRO_SENSITIVITY_2000DPS (0.070F)

typedef enum {
	GYRO_REGISTER_WHO_AM_I = 0x0F,
	GYRO_REGISTER_CTRL_REG1 = 0x20,
	GYRO_REGISTER_CTRL_REG2 = 0x21,
	GYRO_REGISTER_CTRL_REG3 = 0x22,
	GYRO_REGISTER_CTRL_REG4 = 0x23,
	GYRO_REGISTER_CTRL_REG5 = 0x24,
	GYRO_REGISTER_REFERENCE = 0x25,
	GYRO_REGISTER_OUT_TEMP = 0x26,
	GYRO_REGISTER_STATUS_REG = 0x27,  
	GYRO_REGISTER_OUT_X_L = 0x28,
	GYRO_REGISTER_OUT_X_H = 0x29,
	GYRO_REGISTER_OUT_Y_L = 0x2A,
	GYRO_REGISTER_OUT_Y_H = 0x2B,
	GYRO_REGISTER_OUT_Z_L = 0x2C,
	GYRO_REGISTER_OUT_Z_H = 0x2D,
	GYRO_REGISTER_FIFO_CTRL_REG = 0x2E,
	GYRO_REGISTER_FIFO_SRC_REG = 0x2F,
	GYRO_REGISTER_INT1_CFG = 0x30,
	GYRO_REGISTER_INT1_SRC = 0x31,
	GYRO_REGISTER_TSH_XH = 0x32,
	GYRO_REGISTER_TSH_XL = 0x33,
	GYRO_REGISTER_TSH_YH = 0x34,
	GYRO_REGISTER_TSH_YL = 0x35,
	GYRO_REGISTER_TSH_ZH = 0x36,
	GYRO_REGISTER_TSH_ZL = 0x37,
	GYRO_REGISTER_INT1_DURATION = 0x38
} gyroRegisters_t;

typedef enum {
	GYRO_RANGE_250DPS  = 250,
	GYRO_RANGE_500DPS  = 500,
	GYRO_RANGE_2000DPS = 2000
} gyroRange_t;

typedef struct gyroRawData_s {
		int16_t x;
		int16_t y;
		int16_t z;
} gyroRawData_t;

class Adafruit_L3GD20_Unified : public Adafruit_Sensor {
	public:
		Adafruit_L3GD20_Unified(int32_t sensorID = -1);

		bool begin           ( gyroRange_t rng = GYRO_RANGE_250DPS );
		void enableAutoRange ( bool enabled );
		bool getEvent        ( sensors_event_t* );
		void getSensor       ( sensor_t* );

		gyroRawData_t raw; /* Raw values from last sensor read */

	private:
		void        write8  ( byte reg, byte value );
		byte        read8   ( byte reg );
		gyroRange_t _range;
		int32_t     _sensorID;
		bool        _autoRangeEnabled;
};

#endif