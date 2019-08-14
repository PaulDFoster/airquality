#include "PMS.h"
#include <TheThingsNetwork.h>

PMS pms(Serial3);
PMS::DATA data;

// Set your AppEUI and AppKey
  const char *appEui = "70B3D57ED000C467";
  const char *appKey = "94DF4CA5BF1C1E23E2449D24B6B2590F";

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(Serial2, Serial, freqPlan);

volatile int PM_AE_UG_1_0;
volatile int PM_AE_UG_2_5;
volatile int PM_AE_UG_10_0;
static uint8_t mydata[6];

void setup()
{
  Serial.begin(57600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial2.begin(57600);
  Serial3.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  
  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);
  Serial.println("Alive...");

  Serial.println("Startup");
  Serial.println("-- STATUS");
  ttn.showStatus();
  Serial.println("-- JOIN");
  ttn.join(appEui, appKey);

  //pms.passiveMode();    // Switch to passive mode
}

// Removed battery saving because USB charger shuts down
void loop()
{
  //pms.wakeUp();
  delay(30000);

  
//  if (pms.read(data))
  if (pms.readUntil(data))
  {
    PM_AE_UG_1_0 = data.PM_AE_UG_1_0;
    PM_AE_UG_2_5 = data.PM_AE_UG_2_5;
    PM_AE_UG_10_0 = data.PM_AE_UG_10_0;
    
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(PM_AE_UG_10_0);

    Serial.println();

    mydata[0] = highByte(PM_AE_UG_1_0);
    mydata[1] = lowByte(PM_AE_UG_1_0);
    mydata[2] = highByte(PM_AE_UG_2_5);
    mydata[3] = lowByte(PM_AE_UG_2_5);
    mydata[4] = highByte(PM_AE_UG_10_0);
    mydata[5] = lowByte(PM_AE_UG_10_0);
    
    ttn.sendBytes(mydata,sizeof(mydata)); //blocking function
    
    //pms.sleep();
    //ttn.sleep(600000); // 10mins
    delay(600000); // 9 mins because of drift in arduino
    //ttn.wake();
  }

}
