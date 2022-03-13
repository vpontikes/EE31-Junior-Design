#define MPIN0 5
#define MPIN1 6
#define MPIN2 9
#define MPIN3 10

// read cycle: LED OFF, RED LED ON, BLUE LED ON, BOTH LEDS ON
int black[4] = {189,261,252,316};
int red[4] = {222,613,334,667};
int blue[4] = {235,339,531,590};
int yellow[4] = {364,759,628,891};
// tolerance
int diff = 200; 

// move function from 2b
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
  
  Serial.begin( 115200 );
  
  pinMode( A1, INPUT );
  pinMode( 7, OUTPUT );
  pinMode( 8, OUTPUT );

  //analogReadResolution( 16 );
}

bool isBlack(int read[4])
{
  for (int i = 0; i < 4; i++) {
    if (black[i] - read[i] > diff) {
      return false;
    }
    if (black[i] - read[i] < -diff) {
      return false;
    }
  }
  return true;
}

bool isRed(int read[4])
{
  for (int i = 0; i < 4; i++) {
    if (red[i] - read[i] > diff) {
      return false;
    }
    if (red[i] - read[i] < -diff) {
      return false;
    }
  }

  return true;
}

bool isBlue(int read[4])
{

  for (int i = 0; i < 4; i++) {
    if (blue[i] - read[i] > diff) {
      return false;
    }
    if (blue[i] - read[i] < -diff) {
      return false;
    }
  }

  return true;
}

bool isYellow(int read[4])
{

  for (int i = 0; i < 4; i++) {
    if (yellow[i] - read[i] > diff) {
      return false;
    }
    if (yellow[i] - read[i] < -diff) {
      return false;
    }
  }

  return true;
}

void loop()
{
  
  // cycle
  int p7[4] = {0,1,0,1};
  int p8[4] = {0,0,1,1};
  
  int v[4];
  
  // read the 4 values
  for (int i = 0; i < 4; i++) {
    
    digitalWrite( 7, p7[i] );
    digitalWrite( 8, p8[i] );
    
    delay( 10 );
    
    v[i] = analogRead( A1 );
  
    Serial.println(v[i]);
    
    delay( 10 );
  }
  
  move(255,255);
  
  // testing
  if (isBlack(v)) {
    Serial.println( "BLACK" );
  }
  else if (isRed(v)) {
    Serial.println( "RED" );
  }
  else if (isBlue(v)) {
    Serial.println( "BLUE" );
    move(0,0);
    while(1) {
    }
  }
  else if (isYellow(v)) {
    Serial.println( "YELLOW" );
  }
  else {
    Serial.println( "INVALID COLOR" );
  }

}
