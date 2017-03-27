#include <TinyWireM.h>

#define LED_LEFT 0
#define LED_MIDDLE 1
#define LED_RIGHT 2

#define USE_SERIAL

#ifdef USE_SERIAL
	#include <SoftwareSerial.h>
	SoftwareSerial softSerial( 9 , 10 );
#endif

void setup() {

	pinMode( LED_LEFT , OUTPUT );
	pinMode( LED_MIDDLE , OUTPUT );
	pinMode( LED_RIGHT , OUTPUT );

	switchLEDs( HIGH , HIGH , HIGH );
	delay( 500 );
	switchLEDs( LOW , LOW , LOW );

	#ifdef USE_SERIAL
	  	softSerial.begin(9600);
		softSerial.println( "FlexiKurv Prototype" );
	#endif

}

// the loop function runs over and over again forever
void loop() {

	#ifdef USE_SERIAL
		softSerial.print( "Hello World!" );
		delay( 1000 );
  	#endif

}

void switchLEDs( bool leftLED , bool middleLED , bool rightLED ) {
	digitalWrite( LED_LEFT, leftLED );
	digitalWrite( LED_MIDDLE , middleLED );
	digitalWrite( LED_MIDDLE , rightLED );
}
