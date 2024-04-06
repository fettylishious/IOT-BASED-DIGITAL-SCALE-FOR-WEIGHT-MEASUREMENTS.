#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, World!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  delay(10000);
  lcd.setCursor(0, 1);
  lcd.print("i love u");

}
