#include <Wire.h>
#include <SoftwareSerial.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <LiquidCrystalSerial_Slave.h>

SoftwareSerial lcdSerial(3, 4);
hd44780_I2Cexp lcd;

LiquidCrystalSerial_Slave lcsS(&lcdSerial, &lcd);
void setup() {
  lcsS.begin(9600);
}

void loop() {
  lcsS.run();
}
