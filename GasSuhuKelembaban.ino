#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <ESP8266HTTPClient.h>
#include <UrlEncode.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "HEHE TEAM"; // Nama jaringan WiFi
const char *password = "";      // Kata sandi WiFi

const char *server = "api.thingspeak.com";
const unsigned long CHANNEL_ID = 221; // ID Channel Thingspeak
const char *apiKey = "FNNK6Z3";       // Kunci API Thingspeak

const int gasPin = A0;
const int threshold = 900;

const int dhtPin = D6;
const int dhtType = DHT22;

const int buzzerPin = D3;

DHT dht(dhtPin, dhtType);

String temperatureText = "  Suhu: ";
String humidityText = "Lembab: ";
String tempHumidityString = "";
String gasText = "Gas: ";

WiFiClient client;

String phoneNumber = "+628";
String apiKeyChat = "15";

void sendMessage(String message)
{
    String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKeyChat + "&text=" + urlEncode(message);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(url);
    if (httpResponseCode == 200)
    {
        Serial.println("Message sent successfully");
    }
    else
    {
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        lcd.setCursor(0, 0);
        lcd.print("Connecting to WiFi...");
        Serial.println("Connecting WiFi");
        digitalWrite(buzzerPin, HIGH);
    }
    lcd.setCursor(0, 1);
    lcd.print("WiFi Connected:)");
    digitalWrite(buzzerPin, LOW);

    Serial.println("Connected to WiFi");
    delay(1000);
    digitalWrite(buzzerPin, HIGH);

    lcd.clear();

    ThingSpeak.begin(client);

    lcd.print("MQ-2 & DHT22");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
    dht.begin();
}

void loop()
{
    int gasValue = analogRead(gasPin);

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    tempHumidityString = temperatureText + temperature + " C    " + humidityText + humidity + "%    " + gasText + gasValue;
    scrollText(tempHumidityString, 250);

    lcd.clear();

    if (temperature > 50 && humidity > 60)
    {
        lcd.setCursor(0, 0);
        lcd.print("Temperature > 50");
        lcd.setCursor(0, 1);
        lcd.print("humidity > 60%");
        digitalWrite(buzzerPin, LOW);
        sendMessage("Gas Humidity dalam peringatan");
        sendMessage("temperature: " + String(temperature) + "C");
        sendMessage("humidity: " + String(humidity) + "%");
        delay(5000);
        lcd.clear();
        Serial.println("cek tempHumidity nyala dijalankan!");
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("temperature aman");
        lcd.setCursor(0, 1);
        lcd.print("humidity aman");
        digitalWrite(buzzerPin, HIGH);
        sendMessage("Temp dan Humidity aman");
        sendMessage(tempHumidityString);
        delay(3000);
        lcd.clear();
        Serial.println("cek temp dan humidity dijalankan!");
    }

    if (gasValue < threshold)
    {
        lcd.setCursor(0, 0);
        lcd.print("Gas Aman");
        digitalWrite(buzzerPin, HIGH);
        sendMessage("Gas Aman");
        delay(3000);
        lcd.clear();
        Serial.println("cek gas mati dijalankan!");
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Deteksi GAS!");
        digitalWrite(buzzerPin, LOW);
        sendMessage("Deteksi GAS!");
        sendMessage("GasValue: " + gasValue);
        delay(5000);
        lcd.clear();
        Serial.println("cek gas nyala dijalankan!");
    }

    // Kirim data ke Thingspeak
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, gasValue);
    ThingSpeak.setField(3, humidity);

    int response = ThingSpeak.writeFields(CHANNEL_ID, apiKey);
    if (response == 200)
    {
        Serial.println("Data successfully sent to Thingspeak.");
    }
    else
    {
        Serial.println("Error sending data to Thingspeak. HTTP error code: " + String(response));
    }

    delay(10000);
}

void scrollText(String text, int speed)
{
    int textLength = text.length() + 16;
    for (int i = 0; i <= textLength; i++)
    {
        lcd.setCursor(0, 1);
        lcd.print(text.substring(i, i + 16));
        delay(speed);
        lcd.clear();
    }
}
