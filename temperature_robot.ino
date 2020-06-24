#include "ArducamSSD1306.h"    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include "Adafruit_GFX.h"   // Needs a little change in original Adafruit library (See README.txt file)
#include <Wire.h>           // For I2C comm, but needed for not getting compile error
#include <DHT.h>
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int chk;
float humidity;  //Stores humidity value
float tempy; //Stores temperature value

int humidityX = 0;
int humidityY = 0;
int humidityXfactor = 6;
int humidityYfactor = 6;

boolean cf_toggle = true;
int cf_toggle_count = 0;

#define OLED_RESET  -1  // RESET digital signal
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

ArducamSSD1306 display(OLED_RESET); // FOR I2C


void loop() {
  int minimum = 0;
  int maxX = 3;
  int maxY = 3;

  /*
int humidityX = 0;
int humidityY = 0;
int humidityXfactor = 1;
int humidityYfactor = 1;
  */

  humidityX = humidityXfactor + humidityX;
  if(humidityX > 46){
    humidityXfactor = -6;
  }
  if(humidityX < 1){
    humidityXfactor = 6;
  }
  int rx = random(minimum, maxX);
  int ry = random(minimum, maxY);
  
  display.fillScreen(BLACK);
  display_temperature(rx,ry);
  display_humidity(humidityX,LOGO16_GLCD_HEIGHT+20+ry);
  display.display();
  delay(100);
  
}

void display_temperature(int x, int y){
  /*
  boolean cf_toggle = true;
  int cf_toggle_count = 0;
*/
  cf_toggle_count++;
  if(cf_toggle_count > 10){
    cf_toggle_count = 0;
    cf_toggle = !cf_toggle;
  }
  if(cf_toggle){
    display_celsius(x,y);
  }else{
    display_fahrenheit(x,y);
  }
}

void setup(void)
{
	Serial.begin(115200);
  dht.begin();

  // SSD1306 Init
  display.begin();  // Switch OLED
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void display_celsius(int x, int y){
  tempy= dht.readTemperature();
  display.setCursor(x,y);
  display.println(String(tempy) + " c");
}

void display_humidity(int x, int y){
    humidity = dht.readHumidity();
    display.setCursor(x,y);
    display.println(String(humidity) + "%");
}

void display_fahrenheit(int x, int y){
  tempy = dht.readTemperature();
  display.setCursor(x,y);
  display.println(String((int)round(1.8*tempy+32)) + " f");
}
