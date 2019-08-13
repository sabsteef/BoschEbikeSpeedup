/*
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


boolean pulse_was_generated; // previous state
//boolean pulseNow;

// timer settings
unsigned long lastInputPulseAt;
unsigned long lastOutputPulseAt;
unsigned long lastOutputPulse;
unsigned long inputPulsePeriod;
unsigned long nextOutputPulseAt;
unsigned long nextOutputPulse;

//Bike State
bool isOverThreshold;
unsigned long threshold;
unsigned long now;
 bool _pulsePrev = false;

void setup()
{
  pinMode(PULSE_PIN, INPUT);
  digitalWrite(PULSE_PIN, HIGH); // pull-up
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // pull-down
  Serial.begin(115200);
  pulse_was_generated = false;
  Serial.println ("startup");
  
  lastInputPulseAt = 0;
  inputPulsePeriod = 9999999;
  nextOutputPulse = 9999999;  
  
  threshold = 364;
  isOverThreshold = false;
}


boolean HandlePulse()
{
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
    
    lastOutputPulseAt = millis();
    
    Serial.println("Pulse send");
}

boolean handle_pulse(){

  boolean event;
  int pulse_now_generated = !digitalRead(PULSE_PIN); // pin low -> pressed

  event = pulse_now_generated && !pulse_was_generated;
  pulse_was_generated = pulse_now_generated;
  return event;
}

void loop()
{
     now = millis();
     if (handle_pulse())
     {
           inputPulsePeriod = now - lastInputPulseAt;
           lastInputPulseAt = now;
           
           bool oldIsOverThreshold = isOverThreshold;
           isOverThreshold = inputPulsePeriod <= threshold;

           if (!isOverThreshold)
           {
                if(oldIsOverThreshold == true && isOverThreshold == false) return;
                
                relay_pulse();
                Serial.println ("normalmodus");
                return;
           }
           
           nextOutputPulseAt = lastOutputPulse + threshold;
     }
   
     if (isOverThreshold && now >= nextOutputPulseAt)
     {
           relay_pulse();
           nextOutputPulseAt = now + threshold;
                      Serial.println ("nextOutputPulseAt");
           Serial.print (nextOutputPulseAt);
     }
}
