#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// WiFi credentials
const char* ssid = "4G-MIFI-5C5";
const char* password = "1234567890";

// Firebase configuration
#define FIREBASE_HOST "https://esp-8266-data-d0702-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "y9NsftJtIM6bVzL5rHQi6IQpWh85OOIpfafxti5w"

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize HX711
HX711 scale;

float weight;
float calibration_factor = 449640; // Adjust this value to calibrate your scale

// Firebase objects
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  Serial.begin(115200);
  scale.begin(D5, D6);
  scale.set_scale();
  scale.tare(); // Reset the scale to 0
  long zero_factor = scale.read_average(); // Get a baseline reading
//  lcd.init();

  //turn on LCD backlight
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IOT MEASUREMENT");
  lcd.setCursor(1, 1);
  lcd.print("Weighing Scale");
  delay(3000);
  lcd.clear();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("WiFi connected");

  // Initialize Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
}

void loop() {
  delay(500);
  lcd.clear();
  scale.set_scale(calibration_factor); // Adjust to this calibration factor

  weight = scale.get_units(5);

  lcd.setCursor(0, 0);
  lcd.print("MEASURED WEIGHT:");
  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" KG  ");

//  delay(1000);
//  lcd.clear();

  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();

  // Send data to Firebase
  if (Firebase.setFloat(firebaseData, "/weight", weight)) {
    Serial.println("Firebase log sent successfully");
  } else {
    Serial.println("Firebase log sending failed");
    Serial.println(firebaseData.errorReason());
  }

  delay(5000);
}
