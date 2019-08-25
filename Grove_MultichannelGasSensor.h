/******************************************************************************
 * Arduino-Grove-MultichannelGasSensor-driver                                 *
 * --------------------                                                       *
 * Arduino driver for Grove Multichannel Gas sensor (CO, NO2, C2H5OH, H2, NH3 *
 * CH4, C3H8 and C4H10)                                                       *
 * Author: Olivier Staquet                                                    *
 * Last version available on                                                  *
 * https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver/    *
 ******************************************************************************/
#ifndef _GROVE_MULTICHANNELGASSENSOR_H_
#define _GROVE_MULTICHANNELGASSENSOR_H_

#include <Arduino.h>
#include <Wire.h>

// Default parameters
#define MICS6814_DEFAULT_I2C_ADDR               0x04    // I2C slave address to use if not set
#define MICS6814_DEFAULT_STABLE_CYCLE           20      // Number of cycles with low deviation to consider
                                                        // the sample as stable and reliable

// I2C commands
#define MICS6814_CMD_READ_ADC_CH0               0x01
#define MICS6814_CMD_READ_ADC_CH1               0x02
#define MICS6814_CMD_READ_ADC_CH2               0x03
#define MICS6814_CMD_CHANGE_I2C_ADDR            0x05
#define MICS6814_CMD_READ_EEPROM                0x06
#define MICS6814_CMD_SET_R0                     0x07
#define MICS6814_CMD_CONTROL_LED                0x0A
#define MICS6814_CMD_CONTROL_PWR                0x0B

// EEPROM addresses (configuration stored on the Grove Multichannel Gas Sensor)
#define MICS6814_EEPROM_ADDR_SET                0x00
#define MICS6814_EEPROM_ADDR_FACTORY_ADC_CH0    0x02      // Factory setting for R0
#define MICS6814_EEPROM_ADDR_FACTORY_ADC_CH1    0x04      // Factory setting for R0
#define MICS6814_EEPROM_ADDR_FACTORY_ADC_CH2    0x06      // Factory setting for R0
#define MICS6814_EEPROM_ADDR_USER_ADC_CH0       0x08      // User setting for R0 (after calibration)
#define MICS6814_EEPROM_ADDR_USER_ADC_CH1       0x0A      // User setting for R0 (after calibration)
#define MICS6814_EEPROM_ADDR_USER_ADC_CH2       0x0C      // User setting for R0 (after calibration)
#define MICS6814_EEPROM_ADDR_I2C_ADDRESS        0x14      // I2C address stored on the system

enum GasType {CO, NO2, NH3, C3H8, C4H10, CH4, H2, C2H5OH};

class MiCS6814Class {
  public:
    MiCS6814Class(TwoWire& wire);
    virtual ~MiCS6814Class();

    // Start the driver for I2C address slaveAddress
    // By default: use the MICS6814_DEFAULT_I2C_ADDR (0x04)
    int begin(uint8_t slaveAddress = MICS6814_DEFAULT_I2C_ADDR);
    void end();

    // Obtain the firmware version of the Grove Multichannel Gas Sensor
    // Return 0 if unknown, 1 = v1 and 2 = v2
    int getVersion();
    
    // Output the configuration on Serial
    void displayConfig();
    
    // Change the I2C slave address to the newSlaveAddress
    void changeSlaveAddress(uint8_t newSlaveAddress);

    // Sample the data from the sensor
    void sample();

    // Get the gas concentration for a specific gasType
    // (data coming from the last call of sample())
    float get(GasType gasType);

    // Calibrate the sensor
    void warmup();
    void calibrate();

  private:
    // Internal helpers
    unsigned int readOnCommand(uint8_t command);
    unsigned int readOnCommand(uint8_t command, uint8_t parameter);
    int command(uint8_t command, uint8_t parameter);
    void readRsValues();
    int ledOn();
    int ledOff();
    int heaterOn();
    int heaterOff();

    // Internal variables
    TwoWire* _wire;
    uint8_t _slaveAddress;
    int r0ByChannel[3] = {0, 0, 0};
    int rSByChannel[3] = {0, 0, 0};
};

extern MiCS6814Class MiCS6814;

#endif // _GROVE_MULTICHANNELGASSENSOR_H_
