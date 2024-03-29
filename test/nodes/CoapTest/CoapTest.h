/**
 * @file CoapTest.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief CoAP Test Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 15/03/2023
 *
 * @section libraries Libraries
 */

#ifndef __COAPTEST_H
#define __COAPTEST_H

#include <MKRNB.h>
#include <coap-simple.h>
#include <ArduinoHttpClient.h>

#define SLEEP_TIME_MS 40000 // Sleep time, in milliseconds
#define BUF_SIZE 250        // Buffer Size

/**
 * @brief Function for Setting Up Modem and Chipset
 *
 */
void setupModem();

/**
 * @brief Connects to NB-IoT Network
 *
 * @param nbAccess      NB Object
 * @param gprsAccess    GPRS Object
 * @return true         Connection Successful
 * @return false        Connection Failed
 */
bool connectNB(NB &nbAccess, GPRS &gprsAccess);

/**
 * @brief Creates Serialised JSON document for CoAP Test
 *
 * @param testVal   A float used for testing purposes
 * @param buffer    Buffer to store serialised JSON document
 */
void serialiseJson(float &testVal, char *buffer);

/**
 * @brief Sends Packet to CoAP server
 *
 * @param coapServer_ip IP Address to Server (Object)
 * @param coap          Coap Object
 * @param rand          Random Number
 */
void sendPacket(IPAddress &coapServer_ip, Coap &coap, float &rand);

/**
 * @brief Provides IP Address from IP Lookup Server
 *
 * @param ipAddress     IPAddress Object where IP Address will be stored
 * @param httpClient    HTTP Client Object
 */
void getIPAddress(IPAddress &ipAddress, HttpClient &httpClient);

#endif