#define MPIN0 5
#define MPIN1 6
#define MPIN2 9
#define MPIN3 10

void move( int lspeed, int rspeed )
{
	analogWrite( MPIN0, lspeed < 0 ? 0 : lspeed );
	analogWrite( MPIN1, lspeed > 0 ? 0 : -lspeed );

	analogWrite( MPIN2, rspeed < 0 ? 0 : rspeed );
	analogWrite( MPIN3, rspeed > 0 ? 0 : -rspeed );
}

void setup()
{
	pinMode( MPIN0, OUTPUT ); // IC Pin 2 -- Motor 0
	pinMode( MPIN1, OUTPUT ); // IC Pin 7 -- Motor 0

	pinMode( MPIN2, OUTPUT ); // IC Pin 10 -- Motor 1
	pinMode( MPIN3, OUTPUT ); // IC Pin 15 -- Motor 1

	pinMode( 3, OUTPUT );
}

void loop()
{
	// !P2 && !P3 -> brake
	// P2 && !P3 -> forwards
	// !P2 && P3 -> backwards
#if 0
	int m = ( millis() >> 4 ) & 0x1FF;
	int d = ( m & 0x100 ) >> 8;
	m &= 0xFF;
	m = constrain( d ? m : 255 + 128 - m, 128, 255 );
	analogWrite( 3, m );
	move( m, m );
	delay( 10 );
#elif 1
	move( 0, 0 );


#elif 0
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
	int a = 0, b = 250;
	byte ad = 1, bd = -1;

	move( a, b );

	if ( a >= 255 ) ad = -1;
	if ( a <= -255 ) ad = 1;
	a += ad;
	if ( b >= 255 ) bd = -1;
	if ( b <= -255 ) bd = 1;
	b += bd;

	for ( unsigned w = millis() + 2; millis() < w; );
#endif
}
