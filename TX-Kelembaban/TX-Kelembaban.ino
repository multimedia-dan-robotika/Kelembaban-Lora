#include <SPI.h>
#include <LoRa.h>

String Humidity1 ;
 
void setup() 
{    
    Serial.begin(9600);
 
    if (!LoRa.begin(915E6)) 
    {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}
 
void loop() 
{
    int Humidity = analogRead(A0);
    Humidity1 = String(Humidity) ;

    Serial.println("Sending packet: ");
    
    LoRa.beginPacket();
    LoRa.print("Humidity: ");
    LoRa.print(Humidity1);
    LoRa.print("#");
    LoRa.endPacket();
    
//    Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(Humidity1);
    Serial.println("%");

    delay (1000);
}
