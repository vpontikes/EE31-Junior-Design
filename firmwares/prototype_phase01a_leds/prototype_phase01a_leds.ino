#define CLAMP( x, lo, hi ) ( (x) < (lo) ? (lo) : ( (x) > (hi) ? (hi) : (x) ) )

#define PIN_RED_LED 9
#define PIN_GREEN_LED 10
#define PIN_BLUE_LED 6

#define PIN_BLINKSWITCH_1 12
#define PIN_BLINKSWITCH_2 2

#define PIN_BLINKPOT_1 A0
#define PIN_BLINKPOT_2 A1

#define BLINKSTATE_OFF 0
#define BLINKSTATE_ON 1
#define BLINKSTATE_RUN 2
#define BLINKSTATE_SLEEP 3
#define BLINKSTATE_DIAGNOSTIC 4

byte blinkstate = BLINKSTATE_OFF;
byte blinkerrors = 5;

volatile byte blinkswitch_highspeed = 0;

byte blinkpot_1;
byte blinkpot_2;

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
	// Modulated by potentiometers 1 and 2.
	for ( byte i = 0; i < 2; i ++ )
	{
		short d = blinkpot_1 << 1;
		analogWrite( PIN_GREEN_LED, blinkpot_2 );
		delay( CLAMP( 500 - d, 1, 1000 ) );
		analogWrite( PIN_GREEN_LED, LOW );
		delay( CLAMP( 500 + d, 1, 1000 ) );
	}
}

void blink_sleep()
{
	// ``Blink at 4 Hz for 1 sec.''
	// Modified by `blinkswitch_highspeed`, although the 10x speedup is reduced to 5x.
	for ( byte i = 0; i < ( blinkswitch_highspeed ? 40 : 8 ); i ++ )
	{
		digitalWrite( PIN_BLUE_LED, i & 1 ? LOW : HIGH );
		delay( blinkswitch_highspeed ? 25 : 125 );
	}

	// ``Fade for 1 sec.''
	// Arbitrarily tweaked to look good.
	for ( byte f = 200; f > 0; f -- )
	{
		analogWrite( PIN_BLUE_LED, f );
		delay( 5 );
	}

	analogWrite( PIN_BLUE_LED, 0 );
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

void isr_blinkswitch_1()
{
	if ( blinkstate != BLINKSTATE_RUN ) return;

	blinkswitch_highspeed = 1;

	Serial.println( "Entering high speed mode." );
}

void isr_blinkswitch_2()
{
	if ( blinkstate != BLINKSTATE_RUN ) return;
	if ( blinkswitch_highspeed )
	{
		digitalWrite( PIN_RED_LED, HIGH );
		Serial.println( "Enabling red LED." );
	}
	else
	{
		Serial.println( "Use switch #1 first." );
	}

	
	// No way to reset the blue-LED-fast, red-LED-on status is specified.
}

void setup() 
{
	Serial.begin( 115200 );

	pinMode( PIN_RED_LED, OUTPUT );
	pinMode( PIN_GREEN_LED, OUTPUT );
	pinMode( PIN_BLUE_LED, OUTPUT );

	digitalWrite( PIN_RED_LED, LOW );
	digitalWrite( PIN_GREEN_LED, LOW );
	digitalWrite( PIN_BLUE_LED, LOW );

	pinMode( PIN_BLINKSWITCH_1, INPUT );
	pinMode( PIN_BLINKSWITCH_2, INPUT );

	pinMode( PIN_BLINKPOT_1, INPUT );
	pinMode( PIN_BLINKPOT_2, INPUT );

	blinkpot_1 = analogRead( PIN_BLINKPOT_1 );
	blinkpot_2 = analogRead( PIN_BLINKPOT_2 );

	attachInterrupt( digitalPinToInterrupt( PIN_BLINKSWITCH_1 ), isr_blinkswitch_1, RISING );
	attachInterrupt( digitalPinToInterrupt( PIN_BLINKSWITCH_2 ), isr_blinkswitch_2, FALLING );

	Serial.println( "Ready." );
}

void loop()
{
	blinkpot_1 = analogRead( PIN_BLINKPOT_1 );
	blinkpot_2 = analogRead( PIN_BLINKPOT_2 );

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
