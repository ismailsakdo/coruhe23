// id: 2297602, key = 5ACP1C2BJ1UNG72L

//Library
#include <WiFi.h>
#include "ThingSpeak.h"

//Nie amek dari secrete.h
#define SECRET_SSID "XXXXXX"    
#define SECRET_PASS "passsword"
#define SECRET_CH_ID ID 000000000     
#define SECRET_WRITE_APIKEY "APITHINGSPEAK" 

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myStatus = "";

//==================

//Definition
const int lightSensor = 34;
const int irSensor = 35;
const int soundSensor = 12;
const int gasSensor = 4;

// Define the range of raw sensor values
const int rawMin = 55;
const int rawMax = 445;

// Define the corresponding CO2 range
const long co2Min = 1000;
const long co2Max = 400;

//variable
int lightValue;
int irStatus;
int soundValue;
int rawValue;

void setup() {
  Serial.begin(9600);
  pinMode(lightSensor,INPUT);
  pinMode(irSensor, INPUT);
  pinMode(soundSensor, INPUT);
  pinMode(gasSensor, INPUT);

  //=========
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);
}

void loop() {
  //AnalogSensor
  lightValue = analogRead(lightSensor);
  Serial.print("Light Value: ");
  Serial.print(lightValue);

  //IR Sensor
  irStatus = digitalRead(irSensor);
  Serial.print(" IR Status:");
  Serial.print(irStatus);

  //Sound Level
  soundValue = analogRead(soundSensor);
  Serial.print(" Sound Value:");
  Serial.print(soundValue); 

  //MQ Sensor
  rawValue = analogRead(gasSensor);
  long co2Value = mapCO2Value(rawValue);
  Serial.print(" Gas Value:");
  Serial.println(co2Value); 
  
  delay(500);

  //====== WiFi & ThingSpeak=======
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, lightValue); //sensorValue
  ThingSpeak.setField(2, co2Value); //sensorValue
  ThingSpeak.setField(3, soundValue); //sensorValue
  ThingSpeak.setField(4, irStatus); //sensorValue

  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(15000);
  //========Thingspeak=====//
}

// Function to map the CO2 value based on the global range variables
long mapCO2Value(long x) {
  return map(x, rawMin, rawMax, co2Min, co2Max);
}
