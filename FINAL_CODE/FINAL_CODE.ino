//#include <LiquidCrystal_I2C.h>
//#include "HX711.h"
//#include <Wire.h>
//
//LiquidCrystal_I2C lcd(0x27, 16, 2);
//
// 
// 
//HX711 scale;
// 
//float weight;
//float calibration_factor = 449640; // for me this vlaue works just perfect 419640
// 
//void setup() 
//{
//
//  Serial.begin(115200);
//  scale.begin(D5, D6);
//  scale.set_scale();
//  scale.tare(); //Reset the scale to 0
//  long zero_factor = scale.read_average(); //Get a baseline reading
//  lcd.init();
//  // turn on LCD backlight                      
//  lcd.backlight();
//  lcd.setCursor(0,0);
//  lcd.print("IOT MEASURENT");
//  lcd.setCursor(1,1);
//  lcd.print("Weighing Scale");
//  delay(3000);
//  lcd.clear();
//
//} 
// 
//void loop()
//{
// 
//  
//  scale.set_scale(calibration_factor); //Adjust to this calibration factor
// 
//  weight = scale.get_units(5); 
// 
//  lcd.setCursor(0, 0);
//  lcd.print("MEASURED WEIGHT:");
//  lcd.setCursor(0, 1);
//  lcd.print(weight);
//  lcd.print(" KG  ");
//  
//  delay(2000);
//  lcd.clear();
//  
//  Serial.print("Weight: ");
//  Serial.print(weight);
//  Serial.println(" KG");
//  Serial.println();
//
//}








#include <LiquidCrystal_I2C.h>
#include "HX711.h"
//#include <Wire.h>

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
  lcd.print("WEIGHT MEASURENT");
  lcd.setCursor(1,1);
  lcd.print("Weighing Scale");
  delay(3000);
  lcd.clear();
 
  
 
}
 

 
void loop()
{
 
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  weight = scale.get_units(5); 
 
  lcd.setCursor(0, 0);
  lcd.print("MEASURED WEIGHT:");
  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" KG  ");
  
  delay(2000);
  lcd.clear();
  
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" KG");
  Serial.println();
  
 
 
 
}
