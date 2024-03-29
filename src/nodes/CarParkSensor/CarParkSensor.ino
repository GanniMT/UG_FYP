/**
 * @file CarParkSensor.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief Car Park Sensor
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 11/02/2023
 *
 * @section libraries Libraries
 * - Car Park Sensor Header File (./CarParkSensor.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 * -  HC-SR04 Ultrasonic Sensor
 */

#include <ArduinoLowPower.h>
#include <ArduinoHttpClient.h>
#include <MKRNB.h>
#include "CarParkSensor.h"
#include "arduino_secrets.h"
/* ----------------------------------- ARDUINO SECRETS ----------------------------------- */
char pinNumber[] = SECRET_PINNUMBER;
char apn[] = SECRET_GPRS_APN;
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;
char coapEndpoint[] = SECRET_COAP_ENDPOINT;
uint32_t coapPort = SECRET_COAP_PORT;

/* ----------------------------------- LIBRARY OBJECTS ----------------------------------- */
NBClient nbClient;
NBUDP udp;
Coap coap(udp, JSON_BUF_SIZE);
GPRS gprsAccess;
NB nbAccess(false);
IPAddress coapServer_ip;

/* ----------------------------------- VARIABLES ----------------------------------- */
int distReadings[ARRAY_MAX];   // Array which is used for averaging
int distReadings_i = 0;        // Index to be used with distReadings[]
float average;                 // Average of distReadings[]
bool isVehicleParked = false;  // Pretty self explanatory haha

void setup() {
  // 9600 Baudrate
  Serial.begin(9600);

  // Waiting for Serial port to Connect - COMMENT FOR DEPLOYMENT
  while (!Serial)
    ;

  Serial.println("Hello from setup!");

  // HC-SR04 shenanigans
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  distReadings[distReadings_i] = getUltrasonicReading();

  if (distReadings_i == ARRAY_MAX - 1) {
    average = averageArray(distReadings, ARRAY_MAX);
    if (average < PARKED_VEHICLE_THRESHOLD_CM) {
      if (!isVehicleParked) {
        // Indicate that car is ACTUALLY parked and change state
        Serial.println("Car is parked");
        isVehicleParked = true;
        changeSendParkingState(isVehicleParked, nbAccess, gprsAccess, coapServer_ip, nbClient, server, httpPort, coap);
      }
    } else {
      if (isVehicleParked) {
        // Indicate that car is ACTUALLY not parked and change state
        Serial.println("Car is not parked");
        isVehicleParked = false;
        changeSendParkingState(isVehicleParked, nbAccess, gprsAccess, coapServer_ip, nbClient, server, httpPort, coap);
      }
    }
    // After finishing reset index to 0
    distReadings_i = 0;

    Serial.println("I am going to sleep!");

    // Forcing any lines to be printed before sleep
    Serial.flush();
    USBDevice.detach();             // Terminating Serial Connection
    LowPower.sleep(SLEEP_TIME_MS);  // Putting Arduino to sleep
    USBDevice.attach();             // Restarting Serial Connection
    delay(3000);                    // Give time for Serial Connection to take place
    Serial.println("\nI have awoken!");
  } else
    distReadings_i++;
}