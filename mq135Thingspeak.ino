#include <WiFi.h>
#include "ThingSpeak.h"

// Define the analog pin for CO2 sensor
const int co2SensorPin = 34;

// Define the range of raw sensor values
const int rawMin = 800;
const int rawMax = 1800;

// Define the corresponding CO2 range
const long co2Min = 15000;
const long co2Max = 600;

// WiFi
#define SECRET_SSID "XXXX"
#define SECRET_PASS "XXXXX"
#define SECRET_CH_ID XXXXXXX
#define SECRET_WRITE_APIKEY "XXXXXX"

// WiFi Items
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
String myStatus = "";

void setup() {
  // Init the serial port communication
  Serial.begin(115200);

  // WiFi
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Read the raw sensor value
  int rawValue = analogRead(co2SensorPin);

  // Map the raw sensor value to the CO2 range
  long co2Value = mapCO2Value(rawValue);

  // Print the mapped CO2 value to the serial monitor
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print("\t Mapped CO2 Value: ");
  Serial.println(co2Value);

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, co2Value);

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the values
  number1++;
  if (number1 > 99) {
    number1 = 0;
  }

  delay(15000); // Wait 15 seconds to update the channel again
}

// Function to map the CO2 value based on the global range variables
long mapCO2Value(long x) {
  return map(x, rawMin, rawMax, co2Min, co2Max);
}
