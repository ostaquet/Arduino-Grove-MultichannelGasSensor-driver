#include "Grove_MultichannelGasSensor.h"

void setup() {
  // Initialize debug console
  Serial.begin(115200);
  delay(3000);

  // Initialize Grove Multichannel Gas Sensor on default I2C address
  // (to use another I2C address : Mics6814.begin(<I2C address>)
  Serial.println("Initializing...");
  MiCS6814.begin();

  // Warmup the sensor
  Serial.println("Warmup the sensor for 30 minutes...");
  MiCS6814.warmup();

  // Calibrate the sensor
  Serial.println("Start calibration process...");
  MiCS6814.calibrate();

  Serial.println("Sensor calibrated!");
  MiCS6814.end();
}

void loop() {
}
