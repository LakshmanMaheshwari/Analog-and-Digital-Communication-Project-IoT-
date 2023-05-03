
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"
 
WiFiClient client;
 
long myChannelNumber = 2128910;
const char myWriteAPIKey[] = "JRJ2NJZ60EVECRES";

#define DHTPIN D1   // what digital pin we're connected to

#define DHTTYPE DHT22  // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
   WiFi.begin("AQEEL's", "aqeel5253");
   while(WiFi.status() != WL_CONNECTED)
   {
     delay(200);
   }
   ThingSpeak.begin(client);
  randomSeed(analogRead(0));
  pinMode(D0,INPUT);  // IR
  pinMode(D2, INPUT); //LDR
  pinMode(A0, INPUT); //PIR
  pinMode(DHTPIN, INPUT);
  pinMode(D4, OUTPUT);//turning on light
  pinMode(D5, OUTPUT);//turning on AC
  dht.begin();
}
float h;
float t;
int proximity;
int energyWasted;
int light1;
String hum;
String temp;
String humanPresence;
String lights;
String Wastage;
void loop() {
 h = dht.readHumidity();
 t = dht.readTemperature();
 int IR=digitalRead(D0); // IR Sensor output pin connected to D0    
 int motion= analogRead(A0);
 int light= digitalRead(D2);
  if((!IR) && (motion > 500) )  
  {  
   proximity=1;
   energyWasted = 0;
   digitalWrite(D4,HIGH);  
   digitalWrite(D5,HIGH);
  }  
   else  
  {
   proximity=0;
   digitalWrite(D4,LOW);  
   digitalWrite(D5,LOW);
   if (t < 28 || !light)
        energyWasted = 1;
   else
     energyWasted = 0;  
  }  
 light1= !(light);
 hum=String(h);
 temp=String(t);
 humanPresence=String(proximity);
 lights=String(light1);
 Wastage=String(energyWasted);
 Serial.print(humanPresence);
 Serial.print(",");
 Serial.print(temp);
 Serial.print(",");
 Serial.print(hum);
 Serial.print(",");
 Serial.print(lights);
 Serial.print(",");
 Serial.print(Wastage);
 Serial.print('\n');
  ThingSpeak.setField(1, proximity);
 ThingSpeak.setField(2, t);
 ThingSpeak.setField(3, h);
 ThingSpeak.setField(4, !light);
 ThingSpeak.setField(5, energyWasted);
 ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 delay(10000);
}
