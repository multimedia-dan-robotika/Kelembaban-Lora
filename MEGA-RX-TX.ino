/*
   Serial Value 1 = AREA
   Serial Value 2 = MOISTURE PERCENT
*/
#include <elapsedMillis.h>
#include <DS3231.h>

#define WLAtas 9
#define WLBawah 8

#define relayBawahIN1 7 // 2
#define relayBawahIN2 3 // 3
#define relayBawahIN3 6 // 6
#define relayBawahIN4 2 // 7
#define relayAtasIN1  4 // 4
#define relayAtasIN2  5 // 5

DS3231 rtc(SDA, SCL);
Time t;
elapsedMillis readSensorMillis;
elapsedMillis areaRelayOutputMillis;
elapsedMillis reservoirRelayOutputMillis;
elapsedMillis RTCMillis;

unsigned long readSensorInterval            = 5000; // 3000
unsigned long areaRelayOutputInterval       = 3000; // 2000
unsigned long reservoirRelayOutputInterval  = 2000; // 2000
unsigned long RTCInterval                   = 10000; // 10000

String value1, value2;
String moist1, moist2, moist3, moist4;
String area1    = "L1";
String area2    = "L2";
String area3    = "L3";
String area4    = "L4";
String message  = "";

int WLBawahRead, WLAtasRead;

String dataReciver;

const byte onHour   = 13;
const byte onMin    = 0;
const byte offHour  = 13;
const byte offMin   = 30;

void setup()
{
  Serial.begin (115200);
  Serial3.begin (9600); // 9600
  rtc.begin();

  pinMode(WLAtas, INPUT);
  pinMode(WLBawah, INPUT);

  pinMode(relayBawahIN1, OUTPUT);
  pinMode(relayBawahIN2, OUTPUT);
  pinMode(relayBawahIN3, OUTPUT);
  pinMode(relayBawahIN4, OUTPUT);
  pinMode(relayAtasIN1, OUTPUT);
  pinMode(relayAtasIN2, OUTPUT);
  //
  //    digitalWrite (relayAtasIN1, LOW);
  //  digitalWrite (relayAtasIN2, LOW);
  //  digitalWrite (relayBawahIN1, LOW);
  //  digitalWrite (relayBawahIN2, LOW);
  //  digitalWrite (relayBawahIN3, LOW);
  //  digitalWrite (relayBawahIN4, LOW);

  //rtc.setTime(13, 27, 0);
  //rtc.setDate(25, 10, 2022);
}

void loop()
{

  //  Serial.println("data Receiver :");
  //  dataReciver = Serial3.readString();
  if (Serial3.available())
  {
    while (Serial3.available() > 0)
    {
      value1 = Serial3.readStringUntil ('#');
      value2 = Serial3.readStringUntil ('#');
      sensorMap (value1, value2);
    }

  }

  if (readSensorMillis >= readSensorInterval)
  {
    waterLevelRead();
    readSensorMillis = 0;
  }

  if (areaRelayOutputMillis >= areaRelayOutputInterval)
  {
    areaRelayOutput (moist1, relayBawahIN1);
    areaRelayOutput (moist2, relayBawahIN2);
    areaRelayOutput (moist3, relayBawahIN3);
    areaRelayOutput (moist4, relayBawahIN4);
    areaRelayOutputMillis = 0;
  }

  if (reservoirRelayOutputMillis >= reservoirRelayOutputInterval)
  {
    reservoirRelayOutput();
    reservoirRelayOutputMillis = 0;
  }

//  if (RTCMillis >= RTCInterval)
//  {
//    RTCRead (moist1, relayBawahIN1);
//    RTCRead (moist2, relayBawahIN2);
//    RTCRead (moist3, relayBawahIN3);
//    RTCRead (moist4, relayBawahIN4);
//    RTCMillis = 0;
//  }
}


void sensorMap (String area, String sensor)
{
  if (area == area1)
  {
    moist1 = sensor;
  }
  else if (area == area2)
  {
    moist2 = sensor;
  }
  else if (area == area3)
  {
    moist3 = sensor;
  }
  else if (area == area4)
  {
    moist4 = sensor;
  }
}

void waterLevelRead()
{
  WLAtasRead    = digitalRead (WLAtas);
  WLBawahRead   = digitalRead (WLBawah);
}

void areaRelayOutput (String moistValue, int relayArea)
{
  int moistPercentA = moistValue.toInt();
  Serial.println("Soil Value : ");
  Serial.println(moistPercentA);

  if (moistPercentA >= 5 && moistPercentA <= 40)
  {
    //Turn ON Designated Area Relay
    digitalWrite(relayArea, LOW); //high
    Serial.println(" Value Moist : Nyala");
  }
  else if (moistPercentA >= 70) {
    digitalWrite(relayArea, HIGH); // low
    Serial.println("Value Moist : Mati");
  }


}

void reservoirRelayOutput ()
{
  if (WLBawahRead == 1)
  {
    //Turn OFF Pump and Strobe
    digitalWrite(relayAtasIN1, HIGH);
    Serial.println("Value WLBawah : HIGH");
    digitalWrite(relayAtasIN2, LOW);
  }
  else if (WLAtasRead == 1) {
    digitalWrite(relayAtasIN1, HIGH);
    Serial.println("Value WLBawah : HIGH");
    digitalWrite(relayAtasIN2, LOW);
  }
  else {
    //Turn ON Pump and Strobe
    digitalWrite(relayAtasIN1, LOW);
    Serial.println("Value WLBawah : HIGH");
    digitalWrite(relayAtasIN2, HIGH);
  }
}
