#define THRESHOLD 20

void setup()
{
	Serial.begin( 115200 );
	
	pinMode( A2, INPUT );
	pinMode( 2, OUTPUT );
	pinMode( 3, OUTPUT );
	digitalWrite( 2, HIGH );
}

void loop()
{
	int v = analogRead( A2 );
	
	Serial.println( v );

	digitalWrite( 3, v < THRESHOLD );

	delay( 100 );
}
