#ifndef __LSM303_H__
#define __LSM303_H__

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <TinyWireM.h>

#define LSM303_ADDRESS_ACCEL (0x32 >> 1)

typedef enum {
	LSM303_REGISTER_ACCEL_CTRL_REG1_A         = 0x20,   // 00000111   rw
	LSM303_REGISTER_ACCEL_CTRL_REG2_A         = 0x21,   // 00000000   rw
	LSM303_REGISTER_ACCEL_CTRL_REG3_A         = 0x22,   // 00000000   rw
	LSM303_REGISTER_ACCEL_CTRL_REG4_A         = 0x23,   // 00000000   rw
	LSM303_REGISTER_ACCEL_CTRL_REG5_A         = 0x24,   // 00000000   rw
	LSM303_REGISTER_ACCEL_CTRL_REG6_A         = 0x25,   // 00000000   rw
	LSM303_REGISTER_ACCEL_REFERENCE_A         = 0x26,   // 00000000   r
	LSM303_REGISTER_ACCEL_STATUS_REG_A        = 0x27,   // 00000000   r
	LSM303_REGISTER_ACCEL_OUT_X_L_A           = 0x28,
	LSM303_REGISTER_ACCEL_OUT_X_H_A           = 0x29,
	LSM303_REGISTER_ACCEL_OUT_Y_L_A           = 0x2A,
	LSM303_REGISTER_ACCEL_OUT_Y_H_A           = 0x2B,
	LSM303_REGISTER_ACCEL_OUT_Z_L_A           = 0x2C,
	LSM303_REGISTER_ACCEL_OUT_Z_H_A           = 0x2D,
	LSM303_REGISTER_ACCEL_FIFO_CTRL_REG_A     = 0x2E,
	LSM303_REGISTER_ACCEL_FIFO_SRC_REG_A      = 0x2F,
	LSM303_REGISTER_ACCEL_INT1_CFG_A          = 0x30,
	LSM303_REGISTER_ACCEL_INT1_SOURCE_A       = 0x31,
	LSM303_REGISTER_ACCEL_INT1_THS_A          = 0x32,
	LSM303_REGISTER_ACCEL_INT1_DURATION_A     = 0x33,
	LSM303_REGISTER_ACCEL_INT2_CFG_A          = 0x34,
	LSM303_REGISTER_ACCEL_INT2_SOURCE_A       = 0x35,
	LSM303_REGISTER_ACCEL_INT2_THS_A          = 0x36,
	LSM303_REGISTER_ACCEL_INT2_DURATION_A     = 0x37,
	LSM303_REGISTER_ACCEL_CLICK_CFG_A         = 0x38,
	LSM303_REGISTER_ACCEL_CLICK_SRC_A         = 0x39,
	LSM303_REGISTER_ACCEL_CLICK_THS_A         = 0x3A,
	LSM303_REGISTER_ACCEL_TIME_LIMIT_A        = 0x3B,
	LSM303_REGISTER_ACCEL_TIME_LATENCY_A      = 0x3C,
	LSM303_REGISTER_ACCEL_TIME_WINDOW_A       = 0x3D
} lsm303AccelRegisters_t;

typedef struct lsm303AccelData_s {
	int16_t x;
	int16_t y;
	int16_t z;
} lsm303AccelData;

#define LSM303_ID                     (0b11010100)

class Adafruit_LSM303_Accel_Unified : public Adafruit_Sensor {
	public:
		Adafruit_LSM303_Accel_Unified(int32_t sensorID = -1);

		bool begin(void);
		bool getEvent(sensors_event_t*);
		void getSensor(sensor_t*);

		lsm303AccelData raw;   // Last read accelerometer data will be available here

	private:
		int32_t         _sensorID;

		void write8(byte address, byte reg, byte value);
		byte read8(byte address, byte reg);
		void read(void);
};

#endif