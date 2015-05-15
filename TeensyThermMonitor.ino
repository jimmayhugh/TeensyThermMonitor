/********************

TeensyThermMonitor.ino

Version 0.0.1
Last Modified 05/15/2015
By Jim Mayhugh

V-0.0.1 - Initial release

The TeensyThermMonitor is a small board that uses the Teensy-LC board from
http://www.pjrc.com/teensy/teensyLC.html
to monitor up to 4 DS18B20 1-Wire Digital Thermometer, and display the
results on a standard HD44780-compatible 4x20 LCD display.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

This software uses multiple libraries that are subject to additional
licenses as defined by the author of that software. It is the user's
and developer's responsibility to determine and adhere to any additional
requirements that may arise.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


*********************/

#include <OneWire.h>
#include <LiquidCrystalFast.h>


const uint8_t  dsPin      =    9;
const uint8_t  lcdRS      =   12;
const uint8_t  lcdEnable  =   11;
const uint8_t  lcdRW      =   10;
const uint8_t  lcdD4      =    5;
const uint8_t  lcdD5      =    4;
const uint8_t  lcdD6      =    3;
const uint8_t  lcdD7      =    2;
const uint8_t  rows       =    4;
const uint8_t  cols       =   20;
const uint8_t  REDled     =   20;
const uint8_t  GRNled     =   21;
const uint8_t  BLUled     =   22;
const uint8_t  BLled      =   23;

OneWire  ds(dsPin);  // on pin 10 (a 4.7K resistor may be necessary)
LiquidCrystalFast lcd(lcdRS, lcdRW, lcdEnable, lcdD4, lcdD5, lcdD6, lcdD7);

uint8_t data[12];
uint8_t addr[8];
uint8_t x = 0;

char *lcdStr[4] = 
{
  "Water In ",
  "Water Out",
  "Wort In  ",
  "Wort Out "
};

void setup(void)
{
  Serial.begin(115200);
  lcd.begin(cols, rows);
  lcd.clear();
}

void showNext(uint8_t x)
{

  int16_t raw, celsius, fahrenheit;
  uint8_t i;

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(125);               // maybe 750ms is enough, maybe not
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);           // Read Scratchpad
  for ( i = 0; i < 9; i++)  // we need 9 bytes
  {
    data[i] = ds.read();
  }
  raw = (data[1] << 8) | data[0];
  raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  
  celsius = raw / 16;
  fahrenheit = ((celsius * 9) / 5) + 32.0;

  Serial.print("chip");
  Serial.print(x);
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

  lcd.setCursor(0, x);
  lcd.print(lcdStr[x]);
  lcd.setCursor(12, x);
  lcd.print(fahrenheit);
  
}

void loop(void)
{
  if ( !ds.search(addr))
  {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
  }else{
    Serial.print("Displaying Chip ");
    Serial.println(x);
    showNext(x);
    x++;
    if( x >= 4) x = 0;
  }
}




