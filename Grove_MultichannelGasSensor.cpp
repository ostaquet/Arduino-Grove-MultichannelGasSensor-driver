/******************************************************************************
 * Arduino-Grove-MultichannelGasSensor-driver                                 *
 * --------------------                                                       *
 * Arduino driver for Grove Multichannel Gas sensor (CO, NO2, C2H5OH, H2, NH3 *
 * CH4, C3H8 and C4H10)                                                       *
 * Author: Olivier Staquet                                                    *
 * Last version available on                                                  *
 * https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver/    *
 ******************************************************************************/
 
#include "Grove_MultichannelGasSensor.h"

/**
 * Constructor
 * Nothing special, just keep track of the parameters
 */
MiCS6814Class::MiCS6814Class(TwoWire& wire) {
  _wire = &wire;
}

/**
 * Destructor
 * Nothing to do
 */
MiCS6814Class::~MiCS6814Class() {
  
}

/**
 * Start the usage of the driver
 * Return :
 *  - RC 0 if OK
 *  - RC 1 if problem during the transmission attempt
 *  - RC 2 if the device attached at the slave address is not the device expected
 *  - RC 4 if the device has a different version than v2 (only the v2 is supported)
 */
uint8_t MiCS6814Class::begin(uint8_t slaveAddress) {
  // Start the Wire protocol
  _slaveAddress = slaveAddress;
  _wire->begin();

  // Check the initialization with the led (10 attempts)
  bool isReady = false;
  for(uint8_t i = 0; i < 10 && !isReady; i++) {
    isReady = ledOn() == 0;
    delay(50);
    ledOff();
    delay(50);
  }
  
  if(!isReady) {
    return 1;
  }

  // Check the version (works only with v2 of the sensor)
  if(getVersion() != 2) {
    return 4;
  }

  // Turn off heater and LED
  ledOff();
  heaterOff();

  // Get R0 values by channel
  r0ByChannel[0] = readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH0);
  r0ByChannel[1] = readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH1);
  r0ByChannel[2] = readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH2);

  return 0;
}

/**
 * End the usage of the driver
 */
void MiCS6814Class::end() {
  // Cleanup before leaving
  ledOff();
  heaterOff();

  // Close the Wire protocol
  _wire->end();
}

/**
 * Sample the data on the sensor
 */
void MiCS6814Class::sample() {
  // Mark the device as working
  ledOn();
  delay(10);
  // Be sure that the heater is off
  heaterOff();

  // Let the sensor cold down
  delay(1000);

  // Switch on the heater
  heaterOn();
  delay(10);

  uint8_t cycleCount = 1;
  uint8_t stableCycleCount = 0;
  uint8_t prevValueByChannel[3];

  // First reading
  readRsValues();
  prevValueByChannel[0] = rSByChannel[0];
  prevValueByChannel[1] = rSByChannel[1];
  prevValueByChannel[2] = rSByChannel[2];
  delay(1000);

  while(stableCycleCount < MICS6814_DEFAULT_STABLE_CYCLE) {
    readRsValues();
    
    if((abs(prevValueByChannel[0] - rSByChannel[0]) > 2)
      || (abs(prevValueByChannel[1] - rSByChannel[1]) > 2)
      || (abs(prevValueByChannel[2] - rSByChannel[2]) > 2)) {
        prevValueByChannel[0] = rSByChannel[0];
        prevValueByChannel[1] = rSByChannel[1];
        prevValueByChannel[2] = rSByChannel[2];
    } else {
      stableCycleCount++;
    }
    
    cycleCount++;
    delay(1000);
  }

  heaterOff();
  delay(10);
  ledOff();
  delay(10);
}

void MiCS6814Class::warmup() {
  ledOn();
  delay(10);
  heaterOn();
  delay(10);
  
  // For 30 minutes, let the heater on and blink the led
  for(uint8_t min = 29; min >= 0; min--) {
     for(uint8_t sec = 59; sec >= 0; sec--) {
        // Show the user that the warm up is in progress...
        if((min * 60 + sec) % 2 == 0) {
          ledOn();
        } else {
          ledOff();
        }
        delay(1000);
     }
  }

  // Should be good now :-)
  heaterOff();
  delay(10);
  ledOff();
  delay(10);
}

void MiCS6814Class::calibrate() {
  // Sample at usual
  sample();

  // Prepare the data for I2C bus
  uint8_t tmp[6];
  tmp[0] = rSByChannel[0] >> 8;
  tmp[1] = rSByChannel[0] & 0xFF;
  tmp[2] = rSByChannel[1] >> 8;
  tmp[3] = rSByChannel[1] & 0xFF;
  tmp[4] = rSByChannel[2] >> 8;
  tmp[5] = rSByChannel[2] & 0xFF;
  
  // Store the values as R0 on the EEPROM
  Wire.beginTransmission(_slaveAddress);
  Wire.write(MICS6814_CMD_SET_R0);
  for(uint8_t i = 0; i < 6; i++) {
    Wire.write(tmp[i]);
  }
  Wire.endTransmission();

  // Remember the values as R0 in the local instance
  r0ByChannel[0] = rSByChannel[0];
  r0ByChannel[1] = rSByChannel[1];
  r0ByChannel[2] = rSByChannel[2];
}

/**
 * Internal helper to read the Rs values
 */
void MiCS6814Class::readRsValues() {
  rSByChannel[0] = readOnCommand(MICS6814_CMD_READ_ADC_CH0);
  rSByChannel[1] = readOnCommand(MICS6814_CMD_READ_ADC_CH1);
  rSByChannel[2] = readOnCommand(MICS6814_CMD_READ_ADC_CH2);
}

/**
 * Do the math and return the concentration of a specific gas
 * It works only when a sample has been done...
 */
float MiCS6814Class::get(GasType gasType) {
  // Compute the ratio based on R0 and Rs
  float ratio0 = (float)rSByChannel[0]/(float)r0ByChannel[0]*(1023.0-r0ByChannel[0])/(1023.0-rSByChannel[0]);
  float ratio1 = (float)rSByChannel[1]/(float)r0ByChannel[1]*(1023.0-r0ByChannel[1])/(1023.0-rSByChannel[1]);
  float ratio2 = (float)rSByChannel[2]/(float)r0ByChannel[2]*(1023.0-r0ByChannel[2])/(1023.0-rSByChannel[2]);

  float c = 0;

  switch(gasType)
  {
      case CO:
          c = pow(ratio1, -1.179) * 4.385;
          break;
      case NO2:
          c = pow(ratio2, 1.007) / 6.855;
          break;
      case NH3:
          c = pow(ratio0, -1.67) / 1.47;
          break;
      case C3H8:
          c = pow(ratio0, -2.518) * 570.164;
          break;
      case C4H10:
          c = pow(ratio0, -2.138) * 398.107;
          break;
      case CH4:
          c = pow(ratio1, -4.363) * 630.957;
          break;
      case H2:
          c = pow(ratio1, -1.8) * 0.73;
          break;
      case C2H5OH:
          c = pow(ratio1, -1.552)*1.622;
          break;
      default:
          break;
  }

  return isnan(c)?-3:c;
}

/**
 * Change the I2C slave address
 */
void MiCS6814Class::changeSlaveAddress(uint8_t newSlaveAddress) {
  command(MICS6814_CMD_CHANGE_I2C_ADDR, newSlaveAddress);
  _slaveAddress = newSlaveAddress;
  delay(5);
}

/**
 * Display configuration on Serial
 */
void MiCS6814Class::displayConfig() {
  Serial.print("I2C_ADDRESS = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_I2C_ADDRESS));
  Serial.print("FACTORY_ADC_CH0 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_FACTORY_ADC_CH0));
  Serial.print("FACTORY_ADC_CH1 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_FACTORY_ADC_CH1));
  Serial.print("FACTORY_ADC_CH2 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_FACTORY_ADC_CH2));
  Serial.print("USER_ADC_CH0 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH0));
  Serial.print("USER_ADC_CH1 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH1));
  Serial.print("USER_ADC_CH2 = "); Serial.println(readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_USER_ADC_CH2));
}

/**
 * Return the version of the Grove Multichannel Gas Sensor
 * Return values :
 *  - 0 = unable to determine the version
 *  - 1 = version 1
 *  - 2 = version 2
 */
uint8_t MiCS6814Class::getVersion() {
  uint16_t value = readOnCommand(MICS6814_CMD_READ_EEPROM, MICS6814_EEPROM_ADDR_SET);
  if(value == 0x466) {
    return 2;
  } else if(value == 0xFFFF) {
    return 0;
  } else {
    return 1;
  }
}

/**
 * Switch the heater ON
 * Return codes, same as writeRegister()
 */
uint8_t MiCS6814Class::heaterOn() {
  return command(MICS6814_CMD_CONTROL_PWR, 0x01);
}

/**
 * Switch the heater OFF
 * Return codes, same as writeRegister()
 */
uint8_t MiCS6814Class::heaterOff() {
  return command(MICS6814_CMD_CONTROL_PWR, 0x00);
}

/**
 * Switch the LED ON
 * Return codes, same as writeRegister()
 */
uint8_t MiCS6814Class::ledOn() {
  return command(MICS6814_CMD_CONTROL_LED, 0x01);
}

/**
 * Switch the LED OFF
 * Return codes, same as writeRegister()
 */
uint8_t MiCS6814Class::ledOff() {
  return command(MICS6814_CMD_CONTROL_LED, 0x00);
}

/**
 * Read 2 bytes in return of a command sent (without parameter)
 * Return the uint16_t value or 0 by default
 */
uint16_t MiCS6814Class::readOnCommand(uint8_t command) {  
  _wire->beginTransmission(_slaveAddress);
  _wire->write(command);
  _wire->endTransmission();

  delay(5); // Let the time for the bus to react
  _wire->requestFrom(_slaveAddress, (uint8_t) 2);   // Add cast to avoid compile warning ;-)

  uint8_t raw[2];
 
  for (uint8_t i = 0; i < 2; i++) {
    raw[i] = _wire->read();
  }

  uint16_t data = 0;
  data = raw[0];
  data <<= 8;
  data += raw[1];
  return data;
}

/**
 * Read 2 bytes in return of a command sent with a parameter
 * Return the uint16_t or 0 by default
 */
uint16_t MiCS6814Class::readOnCommand(uint8_t command, uint8_t parameter) {  
  _wire->beginTransmission(_slaveAddress);
  _wire->write(command);
  _wire->write(parameter);
  _wire->endTransmission();

  delay(5); // Let the time for the bus to react
  _wire->requestFrom(_slaveAddress, (uint8_t) 2);   // Add cast to avoid compile warning ;-)

  uint8_t raw[2];
 
  for (uint8_t i = 0; i < 2; i++) {
    raw[i] = _wire->read();
  }

  uint16_t data = 0;
  data = raw[0];
  data <<= 8;
  data += raw[1];
  return data;
}

/**
 * Write the value in the register defined by address
 * Return codes:
 *  - 0 OK
 *  - 4 Problem during the transmission
 */
uint8_t MiCS6814Class::command(uint8_t command, uint8_t parameter) {
  _wire->beginTransmission(_slaveAddress);
  _wire->write(command);
  _wire->write(parameter);
  if (_wire->endTransmission() != 0) {
    return 4;
  }
  return 0;
}

// Define the default MiCS6814 object
MiCS6814Class MiCS6814(Wire);
