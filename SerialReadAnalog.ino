// Define the analog pin for CO2 sensor
const int co2SensorPin = 34;

// Define the range of raw sensor values
const int rawMin = 800;
const int rawMax = 1800;

// Define the corresponding CO2 range
const long co2Min = 30000;
const long co2Max = 600;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
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

  // Delay for a moment (adjust as needed)
  delay(1000);
}

// Function to map the CO2 value based on the global range variables
long mapCO2Value(long x) {
  return map(x, rawMin, rawMax, co2Min, co2Max);
}
