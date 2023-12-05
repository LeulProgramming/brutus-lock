#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "utexas-iot"
#define WIFI_PASSWORD "58758837543604326563"
#define API_KEY "AIzaSyABM1CTfoAhMm7n3tJpGcaDEFp6fVtOWuE"

#define USER_EMAIL "jimmy.liang@utexas.edu"
#define USER_PASSWORD "bikess"

#define DATABASE_URL "https://bikelock-afd67-default-rtdb.firebaseio.com/" 

#include <iostream>
#include <vector>

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String uid;

int LDR_PIN = 2;
unsigned long sendDataPrevMillis = 0;
int ldrData = 0;
String message = "Bike is being tampered with!";
String messageNo = "Bike is stationary.";

const int arraySize = 10; // You can change this to the size you need
int Array[arraySize];
int *ptr = Array;
const int resetValue = 10; // The value at which the counter resets
int counter = 0; // Initialize the counter


void setup(){
  pinMode(LDR_PIN,INPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

    /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
}

void loop(){
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Store sensor data to a RTDB
    ldrData = digitalRead(LDR_PIN);
    *ptr = ldrData;
    counter++;
    ptr++;

    if (Firebase.RTDB.setInt(&fbdo, "Sensor/vib_data", ldrData)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (counter == resetValue) {
        std::cout << "Counter reached 10, resetting." << std::endl;
        // Reset counter for 1's
        int countOnes = 0;
        // Count the number of elements equal to 1
        for (int i = 0; i < arraySize; ++i) {
          if (Array[i] == 1) {
          countOnes++;
          } 
        }
    // Check if more than 50% of the elements are equal to 1
    if (countOnes > arraySize / 3) {
      if (Firebase.RTDB.setString(&fbdo, "Sensor/vib_message", message)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      }
      std::cout << "More than 50% of the elements are equal to 1." << std::endl;
    } 
    else {
      if (Firebase.RTDB.setString(&fbdo, "Sensor/vib_message", messageNo)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      }
      std::cout << "Not more than 50% of the elements are equal to 1." << std::endl;
    }
        // Reset the counter
        counter = 0;
        //Reset pointer to first element
        ptr = Array;
    }
    }
  }
