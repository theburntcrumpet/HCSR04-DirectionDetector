#include "HCSR04-Arduino/HCSR04.h"
#define TRIGGER_TOLLERANCE_D 10
#define NUM_PULSES 10
HCSR04 * inRangeSensor;
HCSR04 * outRangeSensor;
enum enCountTypes{COUNT_TYPE_IN=0,COUNT_TYPE_OUT,COUNT_TYPE_NC};
long nInLastDistance  = 0;
long nOutLastDistance = 0;
unsigned long inLastChange;
unsigned long outLastChange;
unsigned long nLastCount = 0;
enCountTypes HandleCount(){
  enCountTypes enRetCount = COUNT_TYPE_NC;
  
  if(inLastChange == 0 || outLastChange == 0)
    return enRetCount;

  if(millis() - nLastCount  < 1500)
    return enRetCount;
  
  if(inLastChange > outLastChange)
    enRetCount = COUNT_TYPE_IN;
  

  if(outLastChange > inLastChange)
    enRetCount = COUNT_TYPE_OUT;

  inLastChange = 0;
  outLastChange = 0;
  nLastCount = millis();
  return enRetCount;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Preparing to start the counting algorithm");
  inRangeSensor = new HCSR04(9,8);
  outRangeSensor = new HCSR04(11,10);
}

void loop() {
   long nCurrentInRange = inRangeSensor->GetAverageDistanceForPulses(NUM_PULSES);
  if(nInLastDistance == 0)
    nInLastDistance = nCurrentInRange;
  else if(nInLastDistance - nCurrentInRange >= TRIGGER_TOLLERANCE_D || 
          nCurrentInRange - nInLastDistance >= TRIGGER_TOLLERANCE_D){
            inLastChange = millis();
          }
  nInLastDistance = nCurrentInRange;

  long nCurrentOutRange = outRangeSensor->GetAverageDistanceForPulses(NUM_PULSES);
  if(nOutLastDistance == 0)
    nOutLastDistance = nCurrentOutRange;
  else if(nOutLastDistance - nCurrentOutRange >= TRIGGER_TOLLERANCE_D || 
          nCurrentOutRange - nOutLastDistance >= TRIGGER_TOLLERANCE_D){
            outLastChange = millis();
          }
  enCountTypes enCurrentCount = HandleCount();
  switch(enCurrentCount){
    case COUNT_TYPE_NC:
      break;
    case COUNT_TYPE_IN:
      Serial.println("IN");
      break;
    case COUNT_TYPE_OUT:
      Serial.println("OUT");
      break;
    default:
      break;
  }
  delay(100);
}
