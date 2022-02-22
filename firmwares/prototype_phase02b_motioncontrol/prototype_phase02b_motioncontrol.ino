void move( int lspeed, int rspeed )
{
	analogWrite( 2, lspeed < 0 ? 0 : lspeed );
	analogWrite( 3, lspeed > 0 ? 0 : -lspeed );

	analogWrite( 4, rspeed < 0 ? 0 : rspeed );
	analogWrite( 5, rspeed > 0 ? 0 : -rspeed );
}

void setup()
{
	pinMode( 2, OUTPUT ); // IC Pin 2 -- Motor 0
	pinMode( 3, OUTPUT ); // IC Pin 7 -- Motor 0

	pinMode( 4, OUTPUT ); // IC Pin 10 -- Motor 1
	pinMode( 5, OUTPUT ); // IC Pin 15 -- Motor 1
}

void loop()
{
	// !P2 && !P3 -> brake
	// P2 && !P3 -> forwards
	// !P2 && P3 -> backwards
#if 0
	static int s;
	s = ( millis() >> 11 ) & 0x3;

	switch ( s )
	{
	case 0: move( 255, 255 ); break;
	case 1: move( 255, 150 ); break;
	case 2: move( 255, 0 ); break;
	case 3: move( 255, -255 ); break;
	default: break;
	}
#else
	if ( ( millis() / 5000 ) & 1 ) move( -254, -254 );
	else move( 0, 0 );
#endif
}
