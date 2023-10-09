// include following, 1) soil, 2) DHT11/ TempRH, 3) IR (digital Sensor)

//Masukkan Library (JIKA BERKENAAN)
#include "DHT.h"

//Initialize/ RENAME The Function (JIKA ADA LIBRARY)/ Asing the PIN
#define SENSOR_IN 0 // update your PIN
#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int pushButton = 2;

//Soil Moisture
const int DryValue = 2590;
const int WetValue = 1430;

//Variable
int soilMoistureValue;
int soilMoisturePercent;

//WiFi


void setup() {
  Serial.begin(9600);

  //Moisture Sensor Start
  //analogReadResolution(12);

  //Hidupkan Sensor DHT11
  dht.begin();

  //Digital sensor
  pinMode(pushButton, INPUT);

  //WiFi
  
}

void loop() {

  //Soil Moisture Value
  soilMoistureValue = analogRead(SENSOR_IN);
  soilMoisturePercent = map(soilMoistureValue, DryValue, WetValue, 0, 100);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);
  Serial.print(soilMoistureValue);
  Serial.print(" - ");
  Serial.println(soilMoisturePercent);
  delay(100);

  //DHT11
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(t);
  Serial.print(" Degree Celcius - ");
  Serial.println(h);
  Serial.print("%");

  //IR Sensor
  int buttonState = digitalRead(pushButton);
  Serial.println(buttonState);
  delay(1);

  //WiFi send data to Thingspeak
  delay(15000);
}
