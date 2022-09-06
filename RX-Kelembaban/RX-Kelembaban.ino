#include <SPI.h>
#include <LoRa.h>

#define CS 10
#define RESET 9
#define DIO0 2

String Humidity1;

void setup() 
{
    Serial.begin(9600);

    LoRa.setPins (CS, RESET, DIO0);
    if (!LoRa.begin(915E6)) 
    {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.receive();
}
 
void loop() 
{
    int packetSize = LoRa.parsePacket();
    if (packetSize) 
    {
        Serial.println("Received packet: ");
        while (LoRa.available()) 
        {
            Humidity1 = LoRa.readStringUntil ('#');
            Serial.print (Humidity1);
            Serial.println("%");
        }
    }
    
    delay (500);
}
