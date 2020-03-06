/*
 Author:
 - Stefan Ringelberg (Stefan.Ringelberg@Gmail.com)
 Hardware:
 - Arduino Nano (Compatible board)
 Software:
 - Arduino 1.8.10

 Arduino Pin Mapping:
 - 00 = Serial RX USB Serial
 - 01 = Serial TX USB Serial
 - 02 = Pulse Receiver ( connect to the speed wire)
 - 03 = Pulse Send ( connect to the engine)
 
*/
#include <Chrono.h> // Control Multitasking

// Definitions
#define PULSE_PIN        2  // Button
#define RELAY_PIN        3  // Button
#define DELAY            5  // Delay per loop in ms

// Instantiate Chronos treats
Chrono chronoA; 
Chrono chronoB; 

boolean pulse_was_generated; // previous state
boolean raising_edge;

// timer settings
unsigned long startTime;
unsigned long endTime;
unsigned long duration;
unsigned long Stop;
unsigned long duration1;
unsigned long Switch;
byte timerRunning;

//Bike State
byte resState;

void setup()
{
  pinMode(PULSE_PIN, INPUT);
  digitalWrite(PULSE_PIN, HIGH); // pull-up
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // pull-down
  Serial.begin(115200);
  pulse_was_generated = false;
  Serial.println ("startup");
  Switch = 400; //hold at 20km showing 17,2
}
     
boolean handle_pulse(){
  boolean event;
   int pulse_now_generated = !digitalRead(PULSE_PIN); // pin low -> pressed
   event = pulse_now_generated && !pulse_was_generated;
   pulse_was_generated = pulse_now_generated;
  return event;
}

void relay_pulse(){
  unsigned long duration;
    digitalWrite(RELAY_PIN, HIGH); // pull-down
    delay(1);
    digitalWrite(RELAY_PIN, LOW); // pull-down
    Serial.println("Pulse send");
}

void loop()
{
    if ( chronoA.hasPassed(1) ) {
     chronoA.restart(); 
     boolean raising_edge = handle_pulse();
     
      if (timerRunning == 1 && raising_edge == 1){
          endTime = millis();
          timerRunning = 0;
          duration1 = endTime - startTime;
          Serial.println ();
          Serial.print ("button press time in milliseconds: ");
          Serial.println (duration1);
          
          if (duration1 <= Switch && Stop !=1 ){
            //duration = duration1 + 600;
            duration = 500; // set to 17,2
           }
          else{
            if (duration != 500){
              duration = duration1;
            }
            else{
              Stop = 1;
            }
          }
          if (duration >= 4000){
            duration = 0;
            Serial.print ("Stop bike ");
          }

     }

     if (timerRunning == 0 && raising_edge == 1){
          startTime = millis();
          timerRunning = 1;
     }
     if ( chronoB.hasPassed(duration) ) {
          if (duration != 0){
            Serial.print ("duration1 ");
           Serial.println (duration1);
               Serial.print ("duration ");
           Serial.println (duration);
          relay_pulse();
          duration = 0;
          Stop =0;
          chronoB.restart();
          }
     }
   }
}
