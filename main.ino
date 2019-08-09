/*
 Author:
 - Stefan Ringelberg (Stefan.Ringelberg@Gmail.com)
 Hardware:
 - Arduino Nano (Compatible board)
 Optional:
 -
 Software:
 - Arduino 1.8.10

 Arduino Pin Mapping:
 - 00 = Serial RX USB Serial
 - 01 = Serial TX USB Serial
 - 02 = Pulse Receiver ( connect to the speed wire)
 - 03 = Pulse Send ( connect to the engine)
 
*/

// Definitions
#define PULSE_PIN        2  // Button
#define RELAY_PIN        3  // Button
#define DELAY            5  // Delay per loop in ms

//Status Flags
#define Normalloop 0
#define Closedloop 1

boolean pulse_was_generated; // previous state
boolean raising_edge;

// timer settings
unsigned long startTime;
unsigned long endTime;
unsigned long duration;
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
}
 

byte NormalLOOP() {
  //Normal loop function. (Sends pulse 1 to 1 to the bike)
  boolean raising_edge = handle_pulse();

 
  if (timerRunning == 1 && raising_edge == 1){
    endTime = millis();
    timerRunning = 0;
    duration = endTime - startTime;
    //Serial.println ();
    //Serial.print ("button press time in milliseconds: ");
    //Serial.println (duration);
  }

  if (timerRunning == 0 && raising_edge == 1){
    startTime = millis();
    timerRunning = 1;
  }

  if (raising_edge != 0 ){
    relay_pulse();
    if (duration <= 363){
      Serial.println("Enter closedloop");
      return Closedloop;
    }
  }

  delay(DELAY);
  return Normalloop;
}


byte ClosedLOOP() {
//Closedloop function. no limit 
  boolean raising_edge = handle_pulse();

  if (timerRunning == 1 && raising_edge == 1){
    endTime = millis();
    timerRunning = 0;
    duration = endTime - startTime;
    //Serial.println ();
    //Serial.print ("button press time in milliseconds: ");
    //Serial.println (duration);
  }

  if (timerRunning == 0 && raising_edge == 1){
    startTime = millis();
    timerRunning = 1;
  }

  if (raising_edge != 0 ){
    if (duration >= 364){
      Serial.println("enter Normalloop");
      return Normalloop;
    }
    delay(350);
    relay_pulse();
  
  }

  delay(DELAY);
  return Closedloop;        
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
    delay(5);
    digitalWrite(RELAY_PIN, LOW); // pull-down
    Serial.println("Pulse send");
}

void loop()
{
  resState = Normalloop;
  // normal loop Bike runs normal
  while (resState == Normalloop) resState = NormalLOOP();
   // Closed loop Bike runs no limit
  while (resState == Closedloop) resState = ClosedLOOP();
}
