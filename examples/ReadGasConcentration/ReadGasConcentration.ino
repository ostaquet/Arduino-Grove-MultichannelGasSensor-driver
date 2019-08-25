#include "Grove_MultichannelGasSensor.h"

void setup() {
  // Initialize debug console
  Serial.begin(115200);
  delay(3000);

  // Initialize Grove Multichannel Gas Sensor on default I2C address
  // (to use another I2C address : Mics6814.begin(<I2C address>)
  Serial.println("Initializing...");
  MiCS6814.begin();
}

void loop() {
  // Sample the data on the sensor
  Serial.println("Sampling in progress...");
  MiCS6814.sample();

  // Output gas concentration
  Serial.print("The concentration of CO is ");
  Serial.print(MiCS6814.get(CO));
  Serial.println(" ppm");
  Serial.print("The concentration of NO2 is ");
  Serial.print(MiCS6814.get(NO2));
  Serial.println(" ppm");
  Serial.print("The concentration of NH3 is ");
  Serial.print(MiCS6814.get(NH3));
  Serial.println(" ppm");
  Serial.print("The concentration of C3H8 is ");
  Serial.print(MiCS6814.get(C3H8));
  Serial.println(" ppm");
  Serial.print("The concentration of C4H10 is ");
  Serial.print(MiCS6814.get(C4H10));
  Serial.println(" ppm");
  Serial.print("The concentration of CH4 is ");
  Serial.print(MiCS6814.get(CH4));
  Serial.println(" ppm");
  Serial.print("The concentration of H2 is ");
  Serial.print(MiCS6814.get(H2));
  Serial.println(" ppm");
  Serial.print("The concentration of C2H50H is ");
  Serial.print(MiCS6814.get(C4H10));
  Serial.println(" ppm");

  // Wait 1 minute before loop
  delay(60000);
}
