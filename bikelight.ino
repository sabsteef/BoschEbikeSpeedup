#include <FastLED.h>  //control LED
#include <Chrono.h>   // Control Multitasking
    
 #define NUM_LEDS 5
 #define DATA_PIN 6
 #define CLOCK_PIN 4
 
CRGB leds[NUM_LEDS];

//consistant values
const int buttonblinkRSwitch = 2;
const int buttonblinkLSwitch = 53;
const int buttonBrakeSwitch  = 22;  

//Values
int buttonblinkR = 0;
int buttonblinkL = 0; 
int buttonBrake = 0;

// Instantiate Chronos treats
Chrono chronoA; 
Chrono chronoB; 
Chrono chronoC;
Chrono chronoX;
int A = 0;

int B = 0;
int C = 0;
 
void setup() {
  // put your setup code here, to run once:
// sanity check delay - allows reprogramming if accidently blowing power w/leds
delay(2000);
//PinLayouts
//imputs
pinMode(buttonblinkRSwitch, INPUT);
pinMode(buttonblinkLSwitch, INPUT);
pinMode(buttonBrakeSwitch, INPUT);

FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RBG>(leds, NUM_LEDS);
FastLED.clear();
//zet achterlight aan
AchterLicht(0,5);
Serial.begin(115200);
Serial.println ("startup");
}


void loop() {
  // check button
  
  buttonblinkR = digitalRead(buttonblinkRSwitch);
  buttonBrake = digitalRead(buttonBrakeSwitch);
  buttonblinkL = digitalRead(buttonblinkLSwitch);

  
  if ( chronoA.hasPassed(10) ) { // returns true if it passed 250 ms since it was started
    chronoA.restart(); // restart the crono so that it triggers again later
    buttonBrake = digitalRead(buttonBrakeSwitch);
    if (buttonBrake == HIGH){
      REMLicht(0,5,0); 
    }
    
  }
  if ( chronoB.hasPassed(200) ) { // returns true if it passed 250 ms since it was started
    chronoB.restart(); // restart the crono so that it triggers again later
    if (buttonblinkL == HIGH){
      AchterKnipperLichtLinks(3,0);
    }  
      else if (buttonblinkL == LOW){
        C = 100;
        
      }
   }
  
 if ( chronoX.hasPassed(101) ) { // returns true if it passed 250 ms since it was started
    chronoX.restart(); // restart the crono so that it triggers again later
    if (buttonBrake == LOW && buttonblinkL == LOW){
      AchterLicht(0,5);
    }  
    
 }
  
 // if (buttonBrake == HIGH){
 //     if(buttonBrake == HIGH && buttonblinkL == HIGH){
 //     REMLicht(2,5,0);
 //     AchterKnipperLichtLinks(2,0);
      
 //     }
 //     else{
 //     REMLicht(0,5,0);
 //     }
 // }
 // else if (buttonblinkR == HIGH){
       //AchterLicht(0,5);
       //AchterKnipperLichtLinks(2,0);
       //REMLicht(0,5);
       //AchterKnipperLichtRechts(0,5);
       //AchterKnipperLichtRechts(2,5);
       //DRLLicht(0,5,64);
       //KnipperLicht(0,5,0);
 //      Police(0,5,0);
 //      delay(600);
      
       
        //FastLED.show(); 
        //delay(30); 
 // }
 // else
 //   DRLLicht(0,5,0);
  //AchterLicht(0,5);
  //AchterLichtOFF(0, 5);

}

void AchterKnipperLichtRechts(int st,int en){
 //FastLED.clear();
  for(int dot = st; dot < en; dot++) { 
     leds[dot] = CRGB::Black;
  }
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Orange;
            FastLED.show();
            delay(200);
            // clear this led for the next time around the loop
            leds[dot] = CRGB::Black;
            delay(200);
        }
}


void AchterKnipperLichtLinks(int st,int en){
 
   if ( chronoC.hasPassed(250) ) { // returns true if it passed 250 ms since it was started
     chronoC.restart(); // restart the crono so that it triggers again later
      //FastLED.clear();
      for(int dot = st; dot >= en; dot--) { 
      leds[dot] = CRGB::Black;
      //FastLED.show();
      }
      A = st;
      B = en;
      
      if (C == 100){
       leds[A] = CRGB::Orange;
       C = A -1;
      }
      else if (C != 100){
        leds[C] = CRGB::Orange;
        if (C == B){ 
        C = 100;
        }
        else{
        C = C -1;
        }
      }
   }
   FastLED.show();
}


void AchterLicht(int st,int en){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Red;
            leds[dot].fadeToBlackBy(164);
            FastLED.show();
        }
}

void REMLicht(int st,int en, int fade){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Red;
            leds[dot].fadeToBlackBy(fade);
            FastLED.show();
        }
}

void AchterLichtOFF(int st,int en){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Black;
            FastLED.show();
        }
}

void DRLLicht(int st,int en, int fade ){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::White;
             leds[dot].fadeToBlackBy(fade);
            FastLED.show();
        }
}


void KnipperLicht(int st,int en, int fade ){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Orange;
             leds[dot].fadeToBlackBy(fade);
            FastLED.show();
  }
  delay(200);
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Black;
             leds[dot].fadeToBlackBy(fade);
            FastLED.show();
  }
        
}
// do not uses ;-)
void Police(int st,int en, int fade ){
  //FastLED.clear(); 
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Blue;
             leds[dot].fadeToBlackBy(fade);
            FastLED.show();
  }
  delay(40);
  for(int dot = st; dot < en; dot++) { 
            leds[dot] = CRGB::Black;
             leds[dot].fadeToBlackBy(fade);
            FastLED.show();
  }
        
}
