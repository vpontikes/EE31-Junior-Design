
#define PIN_RED_LED 9
#define PIN_GREEN_LED 10
#define PIN_BLUE_LED 11

#define PIN_BLINKSWITCH_1 12
#define PIN_BLINKSWITCH_1 13

#define BLINKSTATE_OFF 0
#define BLINKSTATE_ON 1
#define BLINKSTATE_RUN 2
#define BLINKSTATE_SLEEP 3
#define BLINKSTATE_DIAGNOSTIC 4

byte blinkstate = BLINKSTATE_OFF;
byte blinkerrors = 5;

byte blinkswitch_1;
byte blinkswitch_2;

char * blinkstatenames[] = { "OFF", "ON", "RUN", "SLEEP", "DIAGNOSTIC" };

void blink_off()
{
	// Don't blink.
}

void blink_on()
{
	// ``Blink at 10 Hz.''
	digitalWrite( PIN_RED_LED, HIGH );
	delay( 50 );
	digitalWrite( PIN_RED_LED, LOW );
	delay( 50 );
}

void blink_run()
{
	// ``Decay with a time constant of 6 secs.''.
	// Linearly approximated. Actual duration of the fade is (presumably) arbitrary.
	for ( byte f = 175; f > 0; f -- )
	{
		analogWrite( PIN_GREEN_LED, f );
		delay( 10 );
	}

	// Blink twice with ``duty cycle of 0.5 secs''.
	for ( byte i = 0; i < 4; i ++ )
	{
		digitalWrite( PIN_GREEN_LED, i & 1 ? LOW : HIGH );
		delay( 500 );
	}
}

void blink_sleep()
{
	// ``Blink at 4 Hz for 1 sec.''
	for ( byte i = 0; i < 8; i ++ )
	{
		digitalWrite( PIN_BLUE_LED, i & 1 ? LOW : HIGH );
		delay( 125 );
	}

	// ``Fade for 1 sec.''
	for ( byte f = 200; f > 0; f -- )
	{
		analogWrite( PIN_BLUE_LED, f );
		delay( 5 );
	}
	
	blinkstate = BLINKSTATE_OFF;
}

void blink_diagnostic()
{
	// You don't even want to know what the specification says.
	byte b = blinkerrors << 1; // maybe this will be changing dynamically in the future?
	for ( byte i = 0; i < b; i ++ )
	{
		digitalWrite( PIN_RED_LED, i & 1 ? LOW : HIGH );
		delay( 300 );
	}
	delay( 1500 );
}

void ( * blinkstatefunctions[] )() = { blink_off, blink_on, blink_run, blink_sleep, blink_diagnostic };

void setup() 
{
	Serial.begin( 115200 );
	Serial.println( "Serial initialized." );

	pinMode( PIN_RED_LED, OUTPUT );
	pinMode( PIN_GREEN_LED, OUTPUT );
	pinMode( PIN_BLUE_LED, OUTPUT );

	digitalWrite( PIN_RED_LED, LOW );
	digitalWrite( PIN_GREEN_LED, LOW );
	digitalWrite( PIN_BLUE_LED, LOW );
}

void loop()
{
	if ( Serial.available() )
	{
		byte r = Serial.read();
		if ( r == 'f' ) blinkstate = BLINKSTATE_OFF;
		if ( r == 'n' ) blinkstate = BLINKSTATE_ON;
		if ( r == 'r' ) blinkstate = BLINKSTATE_RUN;
		if ( r == 's' ) blinkstate = BLINKSTATE_SLEEP;
		if ( r == 'd' ) blinkstate = BLINKSTATE_DIAGNOSTIC;
		if ( r >= '1' && r <= '9' )
		{
			blinkerrors = r - '0';
			blinkstate = BLINKSTATE_DIAGNOSTIC;
		}
	
		Serial.print( "New state: " );
		Serial.println( blinkstatenames[ blinkstate ] );
	}

	blinkstatefunctions[ blinkstate ]();
}
