/*
 * Magicbit Weather Station 
 * MCU    : Magicbit (ESP32)
 * credit - Tech sayura
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Icons.h"
#define OLED_RESET 4

Adafruit_SSD1306 display(128, 64);

const char* ssid = "WiFi SSID"; //Your WiFi SSID
const char* password = "WiFi key"; //Your WiFi key
String openWeatherMapApiKey = "your API"; //openweathermap.org API key
String city = "city";  //City name
String countryCode = "LK"; //Country Code [LK=Sri Lanka]
String jsonBuffer;
bool res=1;

void frame_1(String desc, String loc, String cntry, String icon_name) { // Current Weather
  display.clearDisplay();
  if (icon_name == "01n") {
    display.drawBitmap(-6, 4, n_01, 64, 64, WHITE);
  } else if (icon_name == "01d") {
    display.drawBitmap(-6, 4, d_01, 64, 64, WHITE);
  } else if (icon_name == "01d") {
    display.drawBitmap(-6, 4, d_01, 64, 64, WHITE);
  } else if (icon_name == "02n") {
    display.drawBitmap(-6, 4, n_02, 64, 64, WHITE);
  } else if (icon_name == "02d") {
    display.drawBitmap(-6, 4, d_02, 64, 64, WHITE);
  } else if ((icon_name == "03n") || (icon_name == "03d")) {
    display.drawBitmap(-6, 4, d_03, 64, 64, WHITE);
  } else if ((icon_name == "04n") || (icon_name == "04d")) {
    display.drawBitmap(-6, 4, d_04, 64, 64, WHITE);
  } else if ((icon_name == "09n") || (icon_name == "09d")) {
    display.drawBitmap(-6, 4, d_04, 64, 64, WHITE);
  } else if (icon_name == "10n") {
    display.drawBitmap(-4, 4, n_10, 64, 64, WHITE);
  } else if (icon_name == "10d") {
    display.drawBitmap(-6, 4, d_10, 64, 64, WHITE);
  } else if ((icon_name == "11n") || (icon_name == "11d")) {
    display.drawBitmap(-6, 4, d_11, 64, 64, WHITE);
  } else if ((icon_name == "13n") || (icon_name == "13d")) {
    display.drawBitmap(-6, 4, d_13, 64, 64, WHITE);
  } else if ((icon_name == "50n") || (icon_name == "50d")) {
    display.drawBitmap(-6, 4, d_50, 64, 64, WHITE);
  }

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("   Current Weather");
  display.setCursor(56, 16);
  display.println(desc);
  display.setCursor(56, 31);
  display.setTextSize(2);
  display.println(cntry);
  display.setCursor(56, 47);
  display.setTextSize(1);
  display.println(loc);
  display.display();

}

void frame_2(float main_temp, float min_temp, float max_temp) { //Tepereture
  display.clearDisplay();
  display.drawBitmap(-20, 4, temp, 64, 64, WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("     Temperature");
  display.setCursor(30, 20);
  display.setTextSize(2);
  display.print(String(main_temp, 1));
  display.setTextSize(2);
  display.print((char)247);
  display.println('C');
  display.setTextSize(1);
  display.setCursor(30, 42);
  display.print("Min : " + String(min_temp, 1));
  display.print((char)247);
  display.println('C');
  display.setCursor(30, 52);
  display.print("Max : " + String(max_temp, 1));
  display.print((char)247);
  display.println('C');
  display.display();
}

void frame_3(float wind_speed) {  //Wind Speed
  display.clearDisplay();
  display.drawBitmap(-14, 4, wind, 64, 64, WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("     Wind Speed");
  display.setCursor(42, 25);
  display.setTextSize(2);
  display.print(String(wind_speed, 1));
  display.print("ms");
  display.setTextSize(1);
  display.println("-1");
  display.display();
}
void frame_4(float humidity) {  //Humidity
  display.clearDisplay();
  display.drawBitmap(-20, 4, hum, 64, 64, WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("       Humidity");
  display.setCursor(45, 25);
  display.setTextSize(2);
  display.print(String(humidity, 1));
  display.println("%");
  display.display();
}

void frame_5(float pressure) { //Atmospheric pressure
  display.clearDisplay();
  display.drawBitmap(-10, 4, pres, 64, 64, WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(" Atmospheric pressure");
  display.setCursor(44, 25);
  display.setTextSize(2);
  display.print(String(pressure, 1));
  display.setTextSize(2);
  display.setCursor(60, 45);
  display.println("hpa");
  display.display();
}

void dicon(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("      Disconected");
  display.drawBitmap(32, 4, nowifi, 64, 64, WHITE);
  display.display();  
}

void displayData(String input) {

  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, input);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }


  JsonObject weather_0 = doc["weather"][0];
  const char* weather_0_main = weather_0["main"];
  const char* weather_0_description = weather_0["description"];
  const char* weather_0_icon = weather_0["icon"];

  const char* base = doc["base"];

  JsonObject main = doc["main"];
  float main_temp = main["temp"];
  float main_temp_min = main["temp_min"];
  float main_temp_max = main["temp_max"];
  int main_pressure = main["pressure"];
  int main_humidity = main["humidity"];

  int visibility = doc["visibility"];

  JsonObject wind = doc["wind"];
  float wind_speed = wind["speed"];
  int wind_deg = wind["deg"];
  float wind_gust = wind["gust"];

  int clouds_all = doc["clouds"]["all"];

  long dt = doc["dt"];

  JsonObject sys = doc["sys"];
  const char* sys_country = sys["country"];
  const char* loc_name = doc["name"];

  frame_1(weather_0_main, loc_name, sys_country, weather_0_icon);
  delay(5000);
  frame_2(main_temp, main_temp_min, main_temp_max);
  delay(5000);
  frame_3(wind_speed);
  delay(5000);
  frame_4(main_humidity);
  delay(5000);
  frame_5(main_pressure);
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(500);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("      Connecting");
  display.drawBitmap(32, 4, wifi, 64, 64, WHITE);
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey + "&units=metric";

    jsonBuffer = httpGETRequest(serverPath.c_str());
    if(jsonBuffer != "{}"){
      displayData(jsonBuffer);
    }else{
      dicon();
    }
  }
  else {
    dicon();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);

  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    payload = http.getString();
  }
  http.end();

  return payload;
}
