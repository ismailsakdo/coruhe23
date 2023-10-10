
//Definition
const int lightSensor = 34;
const int irSensor = 35;
const int soundSensor = 12;
const int gasSensor = 4;

// Define the range of raw sensor values
const int rawMin = 800;
const int rawMax = 1800;

// Define the corresponding CO2 range
const long co2Min = 15000;
const long co2Max = 600;

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
}

// Function to map the CO2 value based on the global range variables
long mapCO2Value(long x) {
  return map(x, rawMin, rawMax, co2Min, co2Max);
}
