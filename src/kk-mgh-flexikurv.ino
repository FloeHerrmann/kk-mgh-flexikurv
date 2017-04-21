#include <TinyWireM.h>

#define USE_SERIAL
//#define USE_LSM303
#define USE_L3GD20


#ifdef USE_SERIAL
	#include <SoftwareSerial.h>
	SoftwareSerial softSerial( 9 , 10 );
#endif

#ifdef USE_LSM303
	#include <Adafruit_Sensor.h>
	#include <Adafruit_LSM303.h>
	Adafruit_LSM303_Accel_Unified lsm303_accel = Adafruit_LSM303_Accel_Unified(54321);
#endif

#ifdef USE_L3GD20
	#include <Adafruit_Sensor.h>
	#include <Adafruit_L3GD20.h>
	Adafruit_L3GD20_Unified l3gd20_gyro = Adafruit_L3GD20_Unified( 20 );
#endif

#define LED_LEFT 0
#define LED_MIDDLE 1
#define LED_RIGHT 2

#define ON_OFF_PIN 3

#define POWER_STATE_ON true
#define POWER_STATE_OFF false

bool powerState = POWER_STATE_OFF;
bool buttonChanged = false;

void setup() {

	pinMode( LED_LEFT , OUTPUT );
	pinMode( LED_MIDDLE , OUTPUT );
	pinMode( LED_RIGHT , OUTPUT );

	pinMode( ON_OFF_PIN , INPUT_PULLUP );

	#ifdef USE_SERIAL
	  	softSerial.begin(9600);
		softSerial.println( "FlexiKurv Prototype" );
	#endif

	#ifdef USE_LSM303
		#ifdef USE_SERIAL
			softSerial.println( "Use LSM303 Accelerometer" );
		#endif

		if( !lsm303_accel.begin() ) {
			#ifdef USE_SERIAL
				softSerial.println("No LSM303 detected!");
			#endif
			while( true );
		}
	#endif

	#ifdef USE_L3GD20
		#ifdef USE_SERIAL
			softSerial.println( "Use L3GD20 Gyroscope" );
		#endif

		l3gd20_gyro.enableAutoRange(true);
		if( !l3gd20_gyro.begin() ) {
			#ifdef USE_SERIAL
				softSerial.println("No L3GD20 detected!");
			#endif
			while( true );
		}
	#endif

}

// the loop function runs over and over again forever
void loop() {

	if( powerState == POWER_STATE_ON ) {

		#if defined( USE_LSM303 ) || defined( USE_L3GD20 )
			sensors_event_t event;
		#endif

		#ifdef USE_LSM303

	    	lsm303_accel.getEvent(&event);

	    	#ifdef USE_SERIAL
				softSerial.print( "X Raw: " ); softSerial.print( lsm303_accel.raw.x ); softSerial.print( "  " );
				softSerial.print( "Y Raw: " ); softSerial.print( lsm303_accel.raw.y ); softSerial.print( "  " );
				softSerial.print( "Z Raw: " ); softSerial.print( lsm303_accel.raw.z ); softSerial.println( "" );

		    	softSerial.print( " -- X: " ); softSerial.print( event.acceleration.x ); softSerial.print( "  " );
		    	softSerial.print( "Y: " ); softSerial.print( event.acceleration.y ); softSerial.print( "  " );
		    	softSerial.print( "Z: " ); softSerial.print( event.acceleration.z ); softSerial.print( "  " );
				softSerial.println( "m/s^2 " );

				delay( 500 );
			#endif
		#endif

		#ifdef USE_L3GD20
	    	l3gd20_gyro.getEvent(&event);

	    	#ifdef USE_SERIAL
				softSerial.print( "X: " ); softSerial.print( event.gyro.x ); softSerial.print( "  " );
	  			softSerial.print( "Y: " ); softSerial.print( event.gyro.y ); softSerial.print( "  " );
	  			softSerial.print( "Z: " ); softSerial.print( event.gyro.z ); softSerial.print( "  " );
	  			softSerial.println("rad/s ");

				delay( 500 );
			#endif
		#endif
	}

	if( digitalRead( ON_OFF_PIN ) == 0 && buttonChanged == false ) {
		buttonChanged = true;
	} else if( digitalRead( ON_OFF_PIN ) == 1 && buttonChanged == true ) {
		buttonChanged = false;
		if( powerState == POWER_STATE_OFF ) {
			#ifdef USE_SERIAL
				softSerial.println( "Turned ON" );
			#endif
			powerState = POWER_STATE_ON;

			switchLEDs( HIGH , HIGH , HIGH ); delay( 300 );
			switchLEDs( LOW , HIGH , LOW ); delay( 300 );

			switchLEDs( HIGH , HIGH , HIGH ); delay( 300 );
			switchLEDs( LOW , HIGH , LOW );

		} else {
			#ifdef USE_SERIAL
				softSerial.println( "Turned OFF" );
			#endif
			powerState = POWER_STATE_OFF;
			switchLEDs( LOW , LOW , LOW );
		}
	}
}

void switchLEDs( bool leftLED , bool middleLED , bool rightLED ) {
	digitalWrite( LED_LEFT, leftLED );
	digitalWrite( LED_MIDDLE , middleLED );
	digitalWrite( LED_RIGHT , rightLED );
}
