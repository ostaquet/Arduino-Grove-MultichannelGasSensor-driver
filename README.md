# Arduino-Grove-MultichannelGasSensor-driver
Arduino library for Grove Multichannel Gas Sensor based on the SGX Sensortech MiCS 6814 (multichannel gas sensor)

This is a comprehensive Arduino library to obtain gas concentration in the air with the (Grove Multichannel Gas Sensor developed by Seeed Studio)[https://www.seeedstudio.com/Grove-Multichannel-Gas-Sensor.html]. The Grove Multichannel Gas Sensor  is an environment detecting sensor with an I2C interface and a built-in MiCS-6814 which can detect many unhealthy gases. Three gases can be measured simultaneously due to its three channels, so it can help you to monitor the concentration of more than one gas.

The detectable gases are listed below (ppm = parts per million):
  * Carbon monoxide (CO) from 1 to 1000 ppm
  * Nitrogen dioxide (NO2) from 0.05 to 10 ppm
  * Ethanol (C2H5OH) from 10 to 500 ppm
  * Hydrogen (H2) from 1 to 1000 ppm
  * Ammonia (NH3) from 1 to 500 ppm
  * Methane (CH4) from >1000 ppm
  * Propane (C3H8) from >1000 ppm
  * Iso-butane (C4H10) from >1000 ppm

The library supports the version 2 of the Grove Multichannel Gas Sensor, the warmup, the calibration, the change of the I2C address, the control of the heater (for low power consumption) and the output of values in ppm (parts per million).

## To know before starting...
  * The library supports only the version 2 of the firmware of the Grove Multichannel Gas Sensor.
  * The Grove Multichannel Gas Sensor works with 3.3V or 5V.
  * The gas sensor MiCS6814 requires minimum 30 minutes preheat time before giving consistent results (also called "burn-in" time or warmup)
  * This library is largely inspired from the original (library Seed-Studio / Multichannel_Gas_Sensor from Jacky Zhang)[https://github.com/Seeed-Studio/Mutichannel_Gas_Sensor/] but the official library didn't work properly with the Arduino Nano 33 IoT.
  
## How to install the library?
 1. Click on *Clone or download* on GitHub
 2. Choose *Download ZIP*
 3. Open Arduino IDE
 4. In the menu *Sketch* -> *Include Library* -> *Add .ZIP Library*
 5. Select the downloaded file or folder (if automatically uncompress)
 6. Restart Arduino IDE
 7. Enjoy!
 
## Circuit
 * GND (black) - Connect to ground
 * VCC (red) - Connect to power supply (3.3V or 5V)
 * SDA (white) - Connect to SDA (I2C data)
 * SCL (yellow) - Connect to SCL (I2C clock)

## Basic programs to use your Grove Multichannel Gas Sensor
In the folder *examples*, you will find various examples to test:
 * (ReadGasConcentration.ino)[] : Sample the gas concentration and show the concentration of all gases supported in ppm.

## Links
 * (Official library of Seeed Studio)[https://github.com/Seeed-Studio/Mutichannel_Gas_Sensor/]
 * (Datasheet sensor SGX Sensortech MiCS 6814)[https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver/blob/master/datasheet/Datasheet%20SGX%20MiCS%206814%20rev8.pdf]
 * (MiCS Application Notes FAQ from SGX Sensortech)[https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver/blob/master/datasheet/FAQ%20for%20SGX%20MiCS%20Gas%20Sensors.pdf]
 * (SGX Sensortech Metal Oxide Gas Sensors - How to use them and how they perform)[https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver/blob/master/datasheet/SGX%20Metal%20Oxide%20Gas%20Sensors%20V1.pdf]
