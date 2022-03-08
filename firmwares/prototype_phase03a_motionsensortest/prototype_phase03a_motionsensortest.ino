void setup()
{
	Serial.begin( 115200 );
	
	pinMode( A1, INPUT );
	pinMode( 7, OUTPUT );
	pinMode( 8, OUTPUT );

	//analogReadResolution( 16 );
}

void loop()
{
	if ( Serial.available() )
	{
		static byte p7 = 0, p8 = 0;

		byte r = Serial.read();
		if ( r == 'a' ) { p7 = 0; p8 = 0; }
		if ( r == 's' ) { p7 = 1; p8 = 0; }
		if ( r == 'd' ) { p7 = 0; p8 = 1; }
		if ( r == 'f' ) { p7 = 1; p8 = 1; }
		
		digitalWrite( 7, p7 );
		digitalWrite( 8, p8 );
	}

	int v = analogRead( A1 );
	
	Serial.println( v );
	//Serial.print( "\x08\x08\x08\x08\x08" );
	delay( 100 );
}
