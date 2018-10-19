/***************************************************************************
                OCTOPOD: A SMART HOME AUTOMATION PROJECT
                      MADE BY SAKSHAM AND ADITYA 
       
Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
Download latest DHT Sensor library here:
    https://github.com/adafruit/DHT-sensor-library
***************************************************************************/

#include <ESP8266WiFi.h>          //Include ESP8266 Wifi Library
#include <BlynkSimpleEsp8266.h>   //Include Blynk Library
#include <DHT.h>                  //Include DHT sensor library

#define BLYNK_PRINT Serial

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a9c89af929024044a8146779d7111aef";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BHUTANI";
char pass[] = "#198s6i8";

#define DHTPIN 2          // What digital pin temperature and humidity sensor is connected to
#define soilPin 4         // What digital pin soil moisture sensor is connected to
#define gasPin A0         // What analog  pin gas sensor is connected to
#define pirPin 12         // What digital pin soil moisture sensor is connected to
 
int pirValue;             // Place to store read PIR Value
int soilValue;            // Place to store read Soil Moisture Value
int PIRpinValue;          // Place to store the value sent by Blynk App Pin V0
int SOILpinValue;         // Place to store the value sent by Blynk App Pin V1

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.


BLYNK_WRITE(V0)             //VO pin from Blynk app tells if Motion Detection is ON
{
 PIRpinValue = param.asInt();    
} 

BLYNK_WRITE(V1)             //V1 pin from Blynk app tells if Soil Moisture is ON
{
 SOILpinValue = param.asInt();    
} 


void sendSensor()
{
  int h = dht.readHumidity();
  int t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!"); // to check if sensor is not sending any false values
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  // send humidity to pin V5
  Blynk.virtualWrite(V6, t);  // send temperature to pin V7
}

void getPirValue(void)
{
  pirValue = digitalRead(pirPin);
  if (pirValue)      //digital pin of PIR gives high value on human detection
  { 
    Serial.println("Motion detected");
    Blynk.notify("Motion detected");  
  }
}

void getSoilValue(void)
{
  soilValue = digitalRead(soilPin);
  if (soilValue == HIGH)     //digital pin of soil sensor give low value when humidity is less
  { 
    Serial.println("Water Plants");
    Blynk.notify("Water Plants");  
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  dht.begin(); // Begins DHT reading
  
  Blynk.tweet("OCTOPOD IS ONLINE!  "); // Tweating on your Twitter Handle that you project is online
  
  pinMode(pirPin,INPUT);    // Defining that Pir Pin is meant to take Input Only
  pinMode(soilPin,INPUT);   // Defining that Soil Sensor Pin is meant to take Input Only
  
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  
  if (PIRpinValue == HIGH)   //VO pin from Blynk app tells if Motion Detection is ON
      {
        getPirValue();
      }
      
  if (SOILpinValue == HIGH)  //V1 pin from Blynk app tells if Soil Moisture is ON
      {
        getSoilValue();
      }
   
}
