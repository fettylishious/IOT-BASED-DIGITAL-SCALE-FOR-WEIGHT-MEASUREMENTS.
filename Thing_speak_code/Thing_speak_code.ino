
#include <HX711.h>
#include <LiquidCrystal_I2C.h>

//#include <ESP8266_LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

const char* ssid = "4G-MIFI-5C5";
const char* password = "1234567890";
const char* server = "api.thingspeak.com";
#define WRITE_API_KEY "CN2U85Z0BA453J6D" //Thingspeak write API key
#define CHANNEL_ID 2549437

WiFiClient client;

LiquidCrystal_I2C lcd(0x27, 16, 2);


 
 
HX711 scale;
 
float weight;
float calibration_factor = 449640; // for me this vlaue works just perfect 419640
 
void setup() 
{

  Serial.begin(115200);
  scale.begin(D5, D6);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("IOT MEASURENT");
  lcd.setCursor(1,1);
  lcd.print("Weighing Scale");
  delay(3000);
  lcd.clear();

  // connect to wifi
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println(".");
  Serial.println("WiFi connected");
//  ThingSpeak.begin(client);
  
 
} 
 
void loop()
{
 
   delay(100);
 lcd.clear();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  weight = scale.get_units(5); 
 
  lcd.setCursor(0, 0);
  lcd.print("MEASURED WEIGHT:");
  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" KG  ");
  
  delay(500);
//  lcd.clear();
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();

  // send data to ThingSpeak
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, weight);
  ThingSpeak.writeFields(CHANNEL_ID,WRITE_API_KEY);
  //delay(500);
 
}
