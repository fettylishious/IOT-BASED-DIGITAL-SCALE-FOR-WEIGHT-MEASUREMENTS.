#include <LiquidCrystal_I2C.h>

#include <SoftwareSerial.h>

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* ssid = "iPhone";
const char* password = "fettyfetty";

// Firebase configuration
#define FIREBASE_HOST "https://warehouse-app-d3b44-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "1ll1tvS0JtiRVulW0kbvgbbOlbGiVUd4XpXjGLhN"


int counter = 0;
LiquidCrystal_I2C lcd(0x27 , 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display


#include "HX711.h"

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// NTP client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000); // Set offset for Tanzania (+3 hours from UTC)

// Define the reset button pin
#define RESET_BUTTON_PIN D3  // Use D3 for GPIO0 on NodeMCU




 
 
HX711 scale;
 
float weight;
float calibration_factor = 449640; // for me this vlaue works just perfect 419640




String getFormattedDateTime() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  int currentHour = (epochTime  % 86400L) / 3600 + 3; // Convert epoch to hours (with timezone offset)
  int currentMinute = (epochTime % 3600) / 60;
  int currentSecond = epochTime % 60;
  
  // Calculate the current day, month, and year
  int currentDay = (epochTime / 86400L) % 30 + 1;
  int currentMonth = (epochTime / 2592000L) % 12 + 1;
  int currentYear = 1970 + (epochTime / 31536000L);

  char formattedTime[20];
  snprintf(formattedTime, sizeof(formattedTime), "%04d-%02d-%02d %02d:%02d:%02d", currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond);
  return String(formattedTime);
}

 
void setup() 
{

  Serial.begin(115200);
  scale.begin(D5, D6);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
//  long zero_factor = scale.read_average(); //Get a baseline reading
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0,0);
   lcd.print("IOT MEASURENT");
  lcd.setCursor(1,1);
  lcd.print("Weighing Scale");
  delay(300);
  lcd.clear();

// Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println();
  Serial.println("WiFi connected");

  // Initialize Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);

  // Initialize NTPClient to get time
  timeClient.begin();
  timeClient.update();

  // Set up a digital input pin for the reset button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  
  
 

} 
 
void loop(){
   delay(3000);
  lcd.clear();
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
 weight = scale.get_units(5);
 lcd.setCursor(0, 0);
  lcd.print("MEASURED WEIGHT:");
  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" KG  ");
  delay(3000);

// Check if reset button is pressed
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(RESET_BUTTON_PIN) == LOW) {
      while (digitalRead(RESET_BUTTON_PIN) == LOW) {
        // Wait for button release to avoid multiple submissions
        delay(10);
      }
      
      if (weight != 0.0) {
        // Get the current time
        String formattedTime = getFormattedDateTime();
        
        // Generate a unique ID using the current timestamp
        String uniqueID = "weight_" + String(millis());

        // Create a JSON object to hold the weight measurement data
        FirebaseJson json;
        json.set("/weight", weight);
        json.set("/time", formattedTime);

        // Send data to Firebase with a unique ID
        String path = "/measurements/" + uniqueID;
        if (Firebase.set(firebaseData, path, json)) {
          Serial.println("Firebase log sent successfully");
        } else {
          Serial.println("Firebase log sending failed");
          Serial.println(firebaseData.errorReason());
        }
      } else {
        Serial.println("Weight is zero, not sending data to Firebase.");
      }
    }
  }

  delay(5000); // Adjust this delay based on how frequently you want to read the weight
}
