// #include <WiFi.h>                 
// #include <FirebaseArduino.h>      
 
// #define FIREBASE_HOST "https://bikelock-afd67-default-rtdb.firebaseio.com/"      
// #define FIREBASE_AUTH "iOoaMksADwvTEl5BRMvBvVZNqGXMJDGsbuXUDxez"            
// #define WIFI_SSID "utexas-iot"                                  
// #define WIFI_PASSWORD "58758837543604326563"            
 
// int vib_pin=2;                                                
 
// // void setup() 
// // {
// //   Serial.begin(115200);
// //   dht.begin();                                                  //reads dht sensor data 
               
// //   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
// //   Serial.print("Connecting to ");
// //   Serial.print(WIFI_SSID);
// //   while (WiFi.status() != WL_CONNECTED) {
// //     Serial.print(".");
// //     delay(500);
// //   }
 
// //   Serial.println();
// //   Serial.print("Connected");
// //   Serial.print("IP Address: ");
// //   Serial.println(WiFi.localIP());                               //prints local IP address
// //   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase
 
// // }

// void setup() {
//   pinMode(vib_pin,INPUT);
//   Serial.begin(9600);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print("\nWiFi connected at IP address ");
//   Serial.println(WiFi.localIP());
// }
 
// void loop() 
// {
//   int val;
//   val=digitalRead(vib_pin);
//   if(val==1)

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "utexas-iot"
#define WIFI_PASSWORD "58758837543604326563"
#define API_KEY "API_KEY"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://bikelock-afd67-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
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

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
  }
}