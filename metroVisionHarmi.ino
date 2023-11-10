//Wifi Library
#include <WiFi.h>
#include "ThingSpeak.h"
#define SECRET_SSID "MySSID"    
#define SECRET_PASS "MyPassword"
#define SECRET_CH_ID 000000
#define SECRET_WRITE_APIKEY "XYZ"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myStatus = "";

#include "Adafruit_PM25AQI.h"
#include <SHT21.h>  // include SHT21 library

#define SENSOR_IN 34

#include <SoftwareSerial.h>
SoftwareSerial pmSerial(16, 17);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
SHT21 sht; 

//variable
int pm1std;
int pm25std;
int pm100std;
float temp;
float humidity;
int Gas;
int GasPercent;

const int Normal = 50; //CO2
const int TakNormal = 80;

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  // Wait one second for sensor to boot up!
  delay(1000);
  pmSerial.begin(9600);
  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");

  //Temperature and Humidity
  Wire.begin();    // begin Wire(I2C)

  //WiFi
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
}

void loop() {

  //MQ135
  Gas = analogRead(SENSOR_IN);
  GasPercent = map(Gas, Normal, TakNormal, 416.55, 1000);
  GasPercent = constrain(GasPercent, 300, 1500);
  Serial.println(GasPercent);

  //Temperature and Humidity
  temp = sht.getTemperature();  // get temp from SHT 
  humidity = sht.getHumidity(); // get temp from SHT

  Serial.print("Temp: ");      // print readings
  Serial.print(temp);
  Serial.print("\t Humidity: ");
  Serial.println(humidity);  

  //Data PM
  PM25_AQI_Data data;

  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }

  pm1std = data.pm10_standard;
  pm25std = data.pm25_standard;
  pm100std = data.pm100_standard;


  Serial.println("AQI reading success");

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(pm1std);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(pm25std);
  Serial.print(F("\t\tPM 10: ")); Serial.println(pm100std);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  delay(1000);


  //Sent Data To ThingSpeak
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
  ThingSpeak.setField(1, pm1std);
  ThingSpeak.setField(2, pm25std);
  ThingSpeak.setField(3, temp);
  ThingSpeak.setField(4, GasPercent);
  
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
  delay(20000); // Wait 20 seconds to update the channel again
}
