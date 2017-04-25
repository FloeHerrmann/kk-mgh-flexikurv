#include <TinyWireM.h>

//#define USE_SERIAL

#ifdef USE_SERIAL
	#include <SoftwareSerial.h>
	SoftwareSerial softSerial( 9 , 10 );
	unsigned long serialHelper = 0;
#endif

#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20.h>
Adafruit_L3GD20_Unified l3gd20_gyro = Adafruit_L3GD20_Unified( 20 );

#define LIGHT_LEFT 0
#define LIGHT_MIDDLE 1
#define LIGHT_RIGHT 2

#define ON_OFF_PIN 3

#define POWER_STATE_ON true
#define POWER_STATE_OFF false

#define GYRO_THRESHOLD_ACTION 10
#define GYRO_THRESHOLD_LEFT -65.0
#define GYRO_THRESHOLD_LEFT_MAX -300.0

#define GYRO_THRESHOLD_RIGHT 65.0
#define GYRO_THRESHOLD_RIGHT_MAX 300.0

bool powerState = POWER_STATE_OFF;
bool buttonChanged = false;

float gyroAbsoluteRotation = 0;
float gyroAbsoluteRotationPrevious = 0;

unsigned long gyroPositionResetTimer = 0;
bool gyroPositionReset = true;

void setup() {

	pinMode( LIGHT_LEFT , OUTPUT );
	pinMode( LIGHT_MIDDLE , OUTPUT );
	pinMode( LIGHT_RIGHT , OUTPUT );

	pinMode( ON_OFF_PIN , INPUT_PULLUP );

	#ifdef USE_SERIAL
	  	softSerial.begin(9600);
		softSerial.println( "FlexiKurv Prototype" );
	#endif

	#ifdef USE_SERIAL
		softSerial.println( "Use L3GD20 Gyroscope" );
	#endif

	l3gd20_gyro.enableAutoRange(true);
	if( !l3gd20_gyro.begin() ) {
		#ifdef USE_SERIAL
			softSerial.println("No L3GD20 detected!");
		#endif
	} else {
		#ifdef USE_SERIAL
			softSerial.println("L3GD20 detected!");
		#endif
	}

	#ifdef USE_SERIAL
		serialHelper = millis();
	#endif
}

// the loop function runs over and over again forever
void loop() {

	// Position/Drehung nur prüfen, wenn "eingeschaltet"
	if( powerState == POWER_STATE_ON ) {

		sensors_event_t event;

	    l3gd20_gyro.getEvent(&event);

		// Momentane Rotation/Position ermitteln bzw. momentane Änderungen aufsummieren
		gyroAbsoluteRotationPrevious = gyroAbsoluteRotation;
		if( event.gyro.z > 0.02 || event.gyro.z < -0.02 ) gyroAbsoluteRotation += event.gyro.z;

		// Momentane Rotation/Position auf 0 zurücksetzen, wenn sich innerhalb eines bestimmten Zeitraums nichts geänder hat
		if( abs( gyroAbsoluteRotation - gyroAbsoluteRotationPrevious ) < GYRO_THRESHOLD_ACTION ) {
			if( gyroPositionReset == false ) gyroPositionResetTimer = millis();
			gyroPositionReset = true;
		} else {
			gyroPositionReset = false;
		}

		// Bei einer Drehung nach Rechts und dem dem überschreiten des Schwellwerts (~25°) die Lampe zuschalten
		if( gyroAbsoluteRotation > GYRO_THRESHOLD_RIGHT && gyroAbsoluteRotation < GYRO_THRESHOLD_RIGHT_MAX ) {
			switchLEDs( HIGH , HIGH , LOW );
			gyroPositionReset = false;
		// Bei einer Drehung nach Links und dem dem überschreiten des Schwellwerts (~25°) die Lampe zuschalten
		} else if( gyroAbsoluteRotation < GYRO_THRESHOLD_LEFT && gyroAbsoluteRotation > GYRO_THRESHOLD_LEFT_MAX ) {
			switchLEDs( LOW , HIGH , HIGH );
			gyroPositionReset = false;
		// Wenn der Lenker wieder in die Mitte gedreht wird die beiden äußeren Lampen abschalten
		} else if( gyroAbsoluteRotation > -20.0 && gyroAbsoluteRotation < 20.0 ) {
			switchLEDs( LOW , HIGH , LOW );
			if( gyroPositionReset == false ) gyroPositionResetTimer = millis();
			gyroPositionReset = true;
		}

		// Momentane Rotation/Position auf 0 zurücksetzen, wenn
		///  - diese sich innerhalb eines bestimmten Zeitraums nichts geänder hat oder
		//   - der Lenker wieder in die Mitte gedreht wurde
		if( gyroPositionReset == true ) {
			if( ( millis() - gyroPositionResetTimer ) > 10000 ) {
				#ifdef USE_SERIAL
					softSerial.println( "Reset Gyro Absolute Position" );
				#endif
				gyroAbsoluteRotation = 0.0;
				gyroPositionReset = false;
			}
		}

	    #ifdef USE_SERIAL
			if( ( millis() - serialHelper ) > 500 ) {
				softSerial.print( "X: " ); softSerial.print( event.gyro.x ); softSerial.print( "  " );
	  			softSerial.print( "Y: " ); softSerial.print( event.gyro.y ); softSerial.print( "  " );
	  			softSerial.print( "Z: " ); softSerial.print( event.gyro.z ); softSerial.print( "  " );
				softSerial.print( "Ztot: " ); softSerial.print( gyroAbsoluteRotation ); softSerial.print( "  " );
	  			softSerial.println("rad/s ");
				serialHelper = millis();
			}
		#endif
	}

	// Licht ein- bzw. ausschalten
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
	digitalWrite( LIGHT_LEFT, leftLED );
	digitalWrite( LIGHT_MIDDLE , middleLED );
	digitalWrite( LIGHT_RIGHT , rightLED );
}
