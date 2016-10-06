/*********************************************************************************
 *                                                                               *
 * AT Tiny Meteor Light                                                          *
 *   By Big Jeff                                                                 *
 *                                                                               *
 *********************************************************************************
 *                                                                               *
 *   Based on a project from BIGCLIVEDOTCOM on youtube.  Original project uses a *
 * PIC 12F629 microcontroller.  The current project is based on an ATtiny85, and *
 * uses regular LED lights.  The LED's are wired up in exactly the same way, and *
 * only the pins on the PIC are really changed.                                  *
 *                                                                               *
 * URL to original project: https://www.youtube.com/watch?v=fR01J0k2IeI          *
 *                                                                               *
 * LEDs are to be hooked up as follows:                                          *
 *   (+) AAABBCBCDCDD                                                            *
 *   (-) BCDCDDAAABBC                                                            *
 *                                                                               *
 * Pins translate to the AT Tiny Pins as follows:                                *
 *   Pin 0 - Line A                                                              *
 *   Pin 1 - Line B                                                              *
 *   Pin 2 - Line C                                                              *
 *   Pin 3 - Reset (short to ground for next effect)                             *
 *   Pin 4 - Line D                                                              *
 *                                                                               *
 *********************************************************************************/


struct myPins {
  byte pHigh;
  byte pLow;
} pinArray[12]={{0,1},{0,2},{0,4},{1,2},{1,4},{2,4},{1,0},{2,0},{4,0},{2,1},{4,1},{4,2}};


int effectIndex=0;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
    //digitalWrite(i,LOW);
}

void loop() {
  if ( digitalRead(3) == LOW ) {
     effectIndex++;
     //if ( effectIndex >= effectMax ) { effectIndex = 0; }
     blinkLight(effectIndex-1, 1000);
  }
  
  // put your main code here, to run repeatedly:
  switch (effectIndex) {
    case 0  : // LED Chase 
        eRainfall(200);
        break;
    case 1  : // LED Chase 2 LED's at a time
        eChase();         
        break;
    case 2 :
        eDoubleChase();   
        break;
    case 3 : 
    case 4 : 
    case 5 :
        eWaterfall(effectIndex, false);
        break;
    case 6 : 
        eWaterfall(3, true);
        break;
    case 7 : 
        eWaterfall(5, true);
        break;
    case 8 : 
        eWaterfall(6, true);
        break;
    case 9 : 
        eAlternate();
        break;
    case 10 : 
        eTest();
        break;
    default : 
        effectIndex = 0;
        blinkLight(0, 1000);
  }
}  

void eRainfall(unsigned long ulFrameRate)
{
  bool bLeds[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  int iLedArray = 1309;
  unsigned long ulTime;
  //int iMask;
  randomSeed(41251897);
 

  do {

    for(int cnt=11;cnt > 0 ;cnt--) {
      bLeds[cnt]= bLeds[cnt-1];
    }

    bLeds[0]= ( random(100) >= 50 );

    ulTime = millis() + ulFrameRate; 

    while ( ulTime > millis() ){  
      for(int cnt=0;cnt < 12;cnt++) {
        if ( bLeds[cnt] ) {
          blinkLight(cnt, 1);
        }
      }
    }

  } while ( digitalRead(3) == HIGH );
}

void eWaterfall(int iLedOn, bool iBottom)
{
  // tracking array
  bool bLeds[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  int maxLed = iLedOn + 12;
  if ( iBottom ) { maxLed--; }
  
  for(int ndx=0; ndx < maxLed; ndx++) {
    if ( ndx < 12 ) {
      bLeds[ndx] = true;
    }

    if ( ndx >= iLedOn ) {
      bLeds[ndx-iLedOn]=false;
    }

    for(int iRpt=0; iRpt < 20; iRpt++){  
      for(int cnt=0;cnt < 12;cnt++) {
        if ( bLeds[cnt] ) {
          blinkLight(cnt, 1);
        }
      }
      if ( iBottom ) {
        blinkLight(11, 1);
      }
    }
     
  }
  
}

void eTest()
{
  for(int cnt=0; cnt<100 ; cnt++) {
    for(int ndx=0; ndx<12 ; ndx++) {
      blinkLight(ndx, 1);
    }
  }
  
}

void eAlternate()
{
  for(int cnt=0; cnt<100 ; cnt++) {
    blinkLight( 0, 1);
    blinkLight( 2, 1);
    blinkLight( 4, 1);
    blinkLight( 6, 1);
    blinkLight( 8, 1);
    blinkLight(10, 1);
  }
  for(int cnt=0; cnt<100 ; cnt++) {
    blinkLight( 1, 1);
    blinkLight( 3, 1);
    blinkLight( 5, 1);
    blinkLight( 7, 1);
    blinkLight( 9, 1);
    blinkLight(11, 1);
  }
}

void eDoubleChase()
{
  for(int ndx=0; ndx<11 ; ndx++) {
    for(int cnt=0; cnt<20 ; cnt++) {
      blinkLight(ndx, 5);
      blinkLight(ndx+1, 5);
    }
  }
}

void eChase()
{
  for(int ndx; ndx<12 ; ndx++) {
    blinkLight(ndx, 100);
  }
}

void blinkLight(int iPin, int bCount)
{
  pinMode(pinArray[iPin].pHigh,OUTPUT);
  digitalWrite(pinArray[iPin].pHigh,HIGH);
  pinMode(pinArray[iPin].pLow,OUTPUT);
  digitalWrite(pinArray[iPin].pLow,LOW);

  delay(bCount);

  pinMode(pinArray[iPin].pHigh,INPUT);
  pinMode(pinArray[iPin].pLow, INPUT);
  
}
