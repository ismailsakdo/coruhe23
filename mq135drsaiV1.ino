#include <MQUnifiedsensor.h>

#define Voltage_Resolution 3.3
#define pin 4
#define type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6
#define co2SamplingInterval 500

MQUnifiedsensor MQ135("Arduino UNO", Voltage_Resolution, ADC_Bit_Resolution, pin, type);
int co2concentrationPPM;

void setup() {
  Serial.begin(9600);
  MQ135.setRegressionMethod(1);
  MQ135.setA(110.47);
  MQ135.setB(-2.862);
  MQ135.init();
  Serial.print("Calibrating, please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");
  if (isinf(calcR0)) {
    Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected). Check wiring and supply.");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Connection issue, R0 is zero (Analog pin shorts to ground). Check wiring and supply.");
    while (1);
  }
}

void loop() {
  MQ135.update();
  co2concentrationPPM = MQ135.readSensor();
  Serial.println(co2concentrationPPM);
  delay(co2SamplingInterval);
}
