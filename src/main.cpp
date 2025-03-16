#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// put your WiFi credentials here:
#define WIFI_NAME "Wokwi-GUEST"
#define WIFI_PASS ""

// put your MQTT credentials here:
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "62051916"

#define MQTT_TEST_TOPIC "62051919/test"

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// function to connect and reconnect as necessary to the MQTT server
void connectWiFi();
void connectMQTT();
void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(115200);
  connectWiFi();
  
  // connect to MQTT
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}

void loop()
{
  connectMQTT();
  delay(500);
}

void connectWiFi()
{
  // connect to WiFi
  Serial.println("Connecting to WiFi: " + String(WIFI_NAME));
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void connectMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_CLIENT_ID))
    {
      Serial.println("connected");
      mqtt.publish(MQTT_TEST_TOPIC, "hello world");
    }
    else
    {
      Serial.print("MQTT Connection Fail, rc= ");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  mqtt.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String top_str, payload_str;
  top_str = topic;
  payload_str = (char *)payload;
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  Serial.println(payload_str);
}
