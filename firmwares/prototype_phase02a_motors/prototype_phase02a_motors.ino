void setup()
{
	pinMode( 6, OUTPUT );
}

void loop()
{
	int m = ( millis() >> 3 ) & 0x1FF;
	int d = ( m & 0x100 ) >> 8;
	m &= 0xFF;
	analogWrite( 6, constrain( d ? m : 255 + 128 - m, 128, 255 ) );
	delay( 10 );
}
