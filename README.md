# IOT-gas_suhu_lembab
Source code IoT-Gas dan Suhu, Kelembaban
Sensor Gas: MQ-2
Sensor Suhu dan Kelembaban: DHT22 
Dibawah ini diperlukannya setup 
- library
- wifi (ssid dan password)
- thingspeak


## Library
- #include <Wire.h>
- #include <LiquidCrystal_I2C.h>
- #include <DHT.h>
- #include <WiFiClient.h>
- #include <ESP8266WiFi.h>
- #include <ThingSpeak.h>
- #include <ESP8266HTTPClient.h>
- #include <UrlEncode.h>

## setup Wifi
- const char* ssid = "HEHE TEAM";      // Nama jaringan WiFi
- const char* password = "bain";   // Kata sandi WiFi

## setup Thingspeak
- const char* server = "api.thingspeak.com";
- const unsigned long CHANNEL_ID = 222;
- const char* apiKey = "FNNK6Z3";    

## setup Whatsapp API
- String phoneNumber = "+628389"; //isikan dengan phoneNumberTarget
- String apiKeyChat = "151"; //isikan dengan apiKeyChat dari callmebot
