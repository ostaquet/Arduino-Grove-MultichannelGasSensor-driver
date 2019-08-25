#include "Grove_MultichannelGasSensor.h"

void setup() {
  // Initialize debug console
  Serial.begin(115200);
  delay(3000);

  // Initialize Grove Multichannel Gas Sensor on default I2C address
  // (to use another I2C address : Mics6814.begin(<I2C address>)
  Serial.println("Initializing...");
  MiCS6814.begin();

  // Display configuration stored on the EEPROM of the Grove Multichannel Gas Sensor
  Serial.println("Current configuration:");
  MiCS6814.displayConfig();

  MiCS6814.end();
}

void loop() {

}
