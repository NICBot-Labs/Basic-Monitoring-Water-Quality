#include <SPI.h>
#include <Ethernet.h>

#define W5500_CS 10

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };

IPAddress ip(192, 168, 10, 10);
IPAddress serverIP(192, 168, 10, 20);

EthernetClient client;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("=== ARDUINO W5500 CLIENT START ===");

  Ethernet.init(W5500_CS);
  Ethernet.begin(mac, ip);

  Serial.print("Hardware Status : ");
  Serial.println(Ethernet.hardwareStatus());

  Serial.print("Link Status     : ");
  Serial.println(Ethernet.linkStatus());

  Serial.print("Local IP        : ");
  Serial.println(Ethernet.localIP());
}

void loop() {

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("WARNING: Ethernet Cable Not Connected");
    delay(1000);
    return;
  }

  if (millis() - lastSend >= 2000) {
    lastSend = millis();

    Serial.println("Connecting to Server...");

    if (client.connect(serverIP, 5000)) {
      Serial.println("Connected!");

      client.println("Hello from Arduino");

      delay(100);

      client.stop();

      Serial.println("Data Sent");
    } else {
      Serial.println("Connection Failed");
    }
  }
}