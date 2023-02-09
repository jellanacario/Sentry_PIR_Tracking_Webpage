//#include <Arduino.h>
//#if defined(ESP32)
//  #include <WiFi.h>
//#elif defined(ESP8266)
//  #include <ESP8266WiFi.h>
//#endif
//#include <Firebase_ESP_Client.h>

#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <string.h>
#include <NTPClient.h>
#include "time.h"

#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h" //Provide the RTDB payload printing info and other helper functions.

//Network Credentials
#define WIFI_SSID "OPPO A52"
#define WIFI_PASSWORD "wintermelon"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDfxktxDujYy_Xgn0zGqWJNEmSGJf--WhY"

#define USER_EMAIL "nacariojella@gmail.com"
#define USER_PASSWORD "pentabyte123"

// Insert RTDB URLefine the RTDB URL */
#define FIREBASE_HOST "esp32-firebase-ee4aa-default-rtdb.asia-southeast1.firebasedatabase.app" 

#define DATABASE_URL "https://esp32-firebase-ee4aa-default-rtdb.asia-southeast1.firebasedatabase.app/"

//FirebaseData firebaseData; //Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


int count = 0;
bool signupOK = false;
WiFiManager wm;

String myString; // sending integer data as string ,sensor data can also be send as integer
String motion = "Motion Detected!";   // string which stores state - motion/no motion
String nomotion="No Motion";
int sdata = 0; // PIR sensor  value will be stored in sdata.
const int sensor = 14;

String uid;

//Main Path
String databasePath;

// Parent Node (to be updated in every loop)
String valuePath = "/Values";
String timePath = "/timestamp";
String parentPath;
String secondParent;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() 
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) 
  {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}


/*
void setup() 
{
  configTime(0, 0, ntpServer);
  Serial.begin(115200);
    pinMode(sensor, INPUT);   // declare sensor as input
    int val = 0;  
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
        {
          Serial.print(".");
          delay(500);
        }
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());

    config.signer.test_mode = true;
    Firebase.reconnectWiFi(true);

    Firebase.begin(&config, &auth);
    databasePath = "/Time/" + uid + "readings/";
}
*/

void setup(){
  Serial.begin(115200);
  pinMode(sensor, INPUT);
  digitalWrite(sensor, LOW);

  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = "AIzaSyDfxktxDujYy_Xgn0zGqWJNEmSGJf--WhY";

  // Assign the user sign in credentials
  auth.user.email = "nacariojella@gmail.com";
  auth.user.password = "pentabyte123";

  // Assign the RTDB URL (required)
  config.database_url = "https://esp32-firebase-ee4aa-default-rtdb.asia-southeast1.firebasedatabase.app/";

  Firebase.reconnectWiFi(true);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";

 
}


void loop()
{
  String value;
  int state = digitalRead(sensor);
  //sdata = digitalRead(sensor);
  myString = String(state); 
  Serial.println(myString);//printing string to verify whether sensor is working or not  
  delay(200);
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    //json.set(valuePath.c_str(), String(state));
    
    //delay(900); 

        //Get current timestamp
        timestamp = getTime();
        Serial.print ("Time: ");
        Serial.println (timestamp);
        delay(100);

         
         //A string will be sent to real time database under variable /value (user need not create this in advance)
         parentPath = databasePath + "/" + String(timestamp);

         //json.set(timePath.c_str(), String(timestamp));
         //json.set(valuePath.c_str(), String(myString));
         
         //secondParent = parentPath + value;
         //Firebase.pushString(firebaseData,parentPath,timestamp);
         //Firebase.pushString(firebaseData,parentPath,state);
         
  
      if(state == HIGH) 
      {
        Serial.println("Motion Detected!"); //Value = 0 which means LOW means no motion is detected
        Serial.println(state); //outputs a value=0 since state is low
        //Firebase.setString(firebaseData,"Sensor State/Status",motion);//A string will be sent to real time database under state- alert -ex -  motion , no motion 

         json.set(timePath.c_str(), String(timestamp));
         json.set(valuePath.c_str(), String(myString));
         
      }
      
      else
      {
        //digitalWrite (Status, LOW);
        Serial.println("No Motion Detected"); //Value = 1 which means HIGH means motion is detected
        Serial.println(state);
        //Firebase.setString(firebaseData,"Sensor State/Status",nomotion);        

         json.set(timePath.c_str(), String(timestamp));
         json.set(valuePath.c_str(), String(myString));

   
      }
      Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
 }
}








/*
void loop() 
{
  String value;
  int state = digitalRead(sensor);
  //sdata = digitalRead(sensor);
  myString = String(state); 
  //Serial.println(myString);//printing string to verify whether sensor is working or not  
  delay(5000);
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    //json.set(valuePath.c_str(), String(state));
    
    //delay(900); 

        //Get current timestamp
        timestamp = getTime();
        Serial.print ("Time: ");
        Serial.println (timestamp);
        delay(5000);

         
         //A string will be sent to real time database under variable /value (user need not create this in advance)
         parentPath = databasePath + "/" + String(timestamp);
         secondParent = parentPath + value;
         Firebase.pushString(firebaseData,parentPath,timestamp);
         Firebase.pushString(firebaseData,parentPath,state);
         
         //json.set(timePath, String(timestamp));
        
      if(state == HIGH) 
      {
        Serial.println("Motion Detected!"); //Value = 1 which means HIGH means motion is detected
        Serial.println(state); //outputs a value=1 since state is high
        Firebase.setString(firebaseData,"Sensor State/Status",motion);//A string will be sent to real time database under state- alert -ex -  motion , no motion 
      }
      else 
      {
        //digitalWrite (Status, LOW);
        Serial.println("No Motion"); //Value = 0 which means LOW means no motion is detected
        Serial.println(state);
        Firebase.setString(firebaseData,"Sensor State/Status",nomotion);        
      }
 }
} */
