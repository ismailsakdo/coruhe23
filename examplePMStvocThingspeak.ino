//Wifi
#include <WiFi.h>
#include "ThingSpeak.h"

#define SECRET_SSID "XXXXX" 
#define SECRET_PASS "XXXXX"

#define SECRET_CH_ID XXXXX     
#define SECRET_WRITE_APIKEY "XXXXX" 

//Wifi Setting
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//Thingspeak
#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(16, 17);

//TVOC sensor
#include <DFRobot_ENS160.h>
#define I2C_COMMUNICATION 
#ifdef  I2C_COMMUNICATION
  DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);
#else
  uint8_t csPin = D3;
  DFRobot_ENS160_SPI ENS160(&SPI, csPin);
#endif

//initializa your variable
int pm1;
int pm25;
int pm03;
int pm05;
int pm11;
int pm255;
uint16_t TVOC;
uint16_t ECO2;
String myStatus = "";

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  delay(1000);
  Serial1.begin(9600);
  pmSerial.begin(9600);
  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");

  // Init the sensor
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);

  //Internet Connection    
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.println("AQI reading success");

  //Naming variable
  pm1=data.pm10_standard;
  pm25=data.pm25_standard;
  pm03=data.particles_03um;
  pm05=data.particles_05um;
  pm11=data.particles_10um;
  pm255=data.particles_25um;

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(pm1);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(pm25);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(pm03);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(pm05);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(pm11);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(pm255);
  Serial.println(F("---------------------------------------"));

  //TVOC reading
  TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  delay(1000);

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

  //Hantar Data
  ThingSpeak.setField(1, pm1);
  ThingSpeak.setField(2, pm25);
  ThingSpeak.setField(3, pm03);
  ThingSpeak.setField(4, pm05);
  ThingSpeak.setField(5, pm11);
  ThingSpeak.setField(6, pm255);
  ThingSpeak.setField(7, TVOC);
  ThingSpeak.setField(8, ECO2);

    
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
}
