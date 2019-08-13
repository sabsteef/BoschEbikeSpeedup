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
boolean raising_edge;

// timer settings
unsigned long startTime;
unsigned long endTime;
unsigned long duration;
byte timerRunning;

     unsinged long lastInputPulseAt;
     unsinged long lastOutputPulseAt;
     unsinged long inputPulsePeriod;
     unsinged long nextOutputPulseAt;
     bool isOverThreshold;
     unsigned long threshold;
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
  
  lastInputPulseAt = 0;
  inputPulsePeriod = 9999999;
  nextOutputPulse = 9999999;  
  
  threshold = 364;
  isOverThreshold = false;
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
     var now = millis();

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
                return;
           }
           
           nextOutputPulseAt = lastOutputPulse + threshold;
     }

     if (isOverThreshold && now >= nextOutputPulseAt)
     {
           relay_pulse();
           nextOutputPulseAt = now + threshold;
     }
}

bool _pulsePrev = false;

public bool HandlePulse()
{
     bool result = false;
     bool pulseNow = IsPinLow();
     result = pulseNow && !_pulsePrev;
     _pulsePrev = pulseNow;
     return result;
}

