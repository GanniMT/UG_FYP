/**
 * @file MQ135.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief MQ135 Sensor Test
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 01/05/2023
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  MQ135 Air Quality Sensor
 */

// Include the library
#include <MQUnifiedsensor.h>

// Definitions
#define board "Arduino UNO"
#define Voltage_Resolution 5
#define pin A1                 // Analog input 0 of your arduino
#define type "MQ-135"          // MQ135
#define ADC_Bit_Resolution 10  // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6 // RS/R0 = 3.6 ppm

// Declare Sensor
MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup()
{
  // Init the serial port communication - to debug the library
  Serial.begin(9600); // Init serial port

  // Waiting for Serial port to Connect
  while (!Serial)
    ;

  // Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(0); // PPM = 10^{[log10(ratio)-b]/a}

  /*****************************  MQ Init ********************************************/
  // Remarks: Configure the pin of arduino as input.
  /************************************************************************************/
  MQ135.init();
  MQ135.setRL(1);
  /*
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ135.setRL(10);
  */
  /*****************************  MQ CAlibration ********************************************/
  // Explanation:
  // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // and on clean air (Calibration conditions), setting up R0 value.
  // We recomend executing this routine only on setup in laboratory conditions.
  // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++)
  {
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  Serial.print("calcR0:");
  Serial.println(calcR0);
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0))
  {
    Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1)
      ;
  }
  if (calcR0 == 0)
  {
    Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1)
      ;
  }
  /*****************************  MQ CAlibration ********************************************/
  Serial.println("** Values from MQ-135 ****");
  Serial.println("|    CO   |   CO2  |");
}

void loop()
{
  MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

  MQ135.setA(-0.220061597);
  MQ135.setB(0.653581876);       // Configure the equation to calculate CO concentration value
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(-0.366725791);
  MQ135.setB(0.763607977);        // Configure the equation to calculate CO2 concentration value
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  Serial.print("|   ");
  Serial.print(CO);
  // Note: 400 Offset for CO2 source: https://github.com/miguel5612/MQSensorsLib/issues/29
  /*
  Motivation:
  We have added 400 PPM because when the library is calibrated it assumes the current state of the
  air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
  https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
  Serial.print("   |   ");
  Serial.print(CO2 + 400);
  Serial.println("   |");

  delay(500); // Sampling frequency
}