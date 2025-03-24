#include <WiFiNINA.h>
#include <DHT.h>
#include <ThingSpeak.h>


// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT11

// WiFi and ThingSpeak Setup
#include "secrets.h"  
WiFiClient client;

// Create DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// ThingSpeak Field Numbers (use different fields for temperature, humidity, etc.)
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(9600);   // Start the Serial Monitor
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
  //Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check for failed readings
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read DHT sensor");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Write to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // Update ThingSpeak every 60 seconds
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200) {
    Serial.println("ThingSpeak Channel successfully updated.");
  } else {
    Serial.println("Error updating ThingSpeak Channel. Status: " + String(x));
  }

  // Wait for 60 seconds before sending the next update
  delay(60000); 
}
