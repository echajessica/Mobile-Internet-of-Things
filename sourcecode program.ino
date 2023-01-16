#include <WiFiClient.h>
#include <ThingSpeak.h>            
#include <WiFi.h> 

const char* ssid     = "Nacha";
const char* password = "FAMILY100";

const char* host = "api.thingspeak.com";
const char* APIKey = "G5JSDMBVH92RA5JM";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // koneksi wifi
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int value = 0;
void loop()
{
    delay(5000);
    value = analogRead(A0);

    Serial.print("connecting to ");
    Serial.println(host);

    // wifi client untuk koneksi tcp
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // request URI
    String url = "https://api.thingspeak.com/update?api_key=G5JSDMBVH92RA5JM&field1=0";
    url += "?api_key=";
    url += APIKey;
    url += "&field1=";
    url += value;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}
