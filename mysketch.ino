#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char *ssid = "jumanji";
const char *password = "8949@pavan";

// MQTT Broker
const char *mqtt_server = "10.0.0.52";
const int mqtt_port = 1883;

// Sound sensor analog pin
#define SOUND_SENSOR_A_PIN 36 // GPIO36 (ADC1)

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ Wi-Fi connected.");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ESP32_Sound_Client"))
        {
            Serial.println("connected.");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(SOUND_SENSOR_A_PIN, INPUT);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
}

void loop()
{
    if (!client.connected())
        reconnect();
    client.loop();

    int soundLevel = analogRead(SOUND_SENSOR_A_PIN); // Range: 0 - 4095
    Serial.print("📈 Sound level: ");
    Serial.println(soundLevel);

    // Convert to string for MQTT
    char soundStr[10];
    snprintf(soundStr, sizeof(soundStr), "%d", soundLevel);
    client.publish("home/sensors/sound", soundStr);

    delay(2000); // Send every 2 seconds
}
