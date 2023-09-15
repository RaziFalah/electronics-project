#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//LCD Object (0x3F or 0x27)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//Put your WiFi Credentials here
const char* ssid = "Wokwi-GUEST";
const char* password = "";

//URL Endpoint for the API
String URL = "http://api.openweathermap.org/data/2.5/weather?";
String ApiKey = "e4601654a66d2e3f42d1f1783b5f5a09";

// Replace with your location Credentials
String lat = "32.7776";
String lon = "35.3057";

void setup() {
  Serial.begin(115200);

  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL with Location and API key information
    http.begin(URL + "lat=" + lat + "&lon=" + lon + "&units=metric&appid=" + ApiKey);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve some information about the weather from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Display the Current Weather Info
      const char* description = obj["weather"][0]["description"].as<const char*>();
      const float temp = obj["main"]["temp"].as<float>();
      const float humidity = obj["main"]["humidity"].as<float>();

      Serial.println(description);
      Serial.println(temp);
      Serial.println(" C, ");
      Serial.println(humidity);
      Serial.println(" %");

    } else {
      Serial.println("Error!");
      lcd.clear();
      lcd.print("Can't Get DATA!");
    }

    http.end();

  }
  
  //Wait for 30 seconds
  delay(30000);
}
