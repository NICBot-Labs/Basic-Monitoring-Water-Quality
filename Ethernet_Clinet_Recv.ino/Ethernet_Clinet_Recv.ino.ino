#include <SPI.h>
#include <Ethernet.h>

#define W5500_CS 5

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
IPAddress ip(192, 168, 10, 20);

EthernetServer server(5000);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== ESP32 W5500 SERVER START ===");

  SPI.begin(18, 19, 23, W5500_CS);   // SCK, MISO, MOSI, CS
  Ethernet.init(W5500_CS);

  Ethernet.begin(mac, ip);

  Serial.print("Hardware Status : ");
  Serial.println(Ethernet.hardwareStatus());

  Serial.print("Link Status     : ");
  Serial.println(Ethernet.linkStatus());

  Serial.print("Local IP        : ");
  Serial.println(Ethernet.localIP());

  server.begin();

  Serial.println("TCP Server Started");
}

void loop() {

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("WARNING: Ethernet Cable Not Connected");
    delay(1000);
    return;
  }

  EthernetClient client = server.available();

  if (client) {
    Serial.println("Client Connected");

    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println("\nClient Disconnected");
  }
}