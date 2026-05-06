#define DEBUG true  // DEBUG ON
#if DEBUG
#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)
#else
#define PRINT(x)
#define PRINTLN(x)
#endif

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "NCBot Official";
const char* password = "SelamatDatang";

float waterTemp;
float tdsValue;
float phValue;
float ntuValue;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  PRINTLN("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    PRINT('.');
    delay(1000);
  }
  PRINTLN(WiFi.localIP());
}

int wifiPercent(int rssi) {
  if (rssi <= -100) return 0;
  if (rssi >= -50) return 100;
  return 2 * (rssi + 100);
}

void sendToServer(float temp, float tds, float ph, float ntu) {
  if (WiFi.status() == WL_CONNECTED) {
    int rssi = WiFi.RSSI();
    int percent = wifiPercent(rssi);

    PRINT(" dBm | Signal: ");
    PRINT(percent);
    PRINTLN("%");

    WiFiClientSecure client;
    client.setInsecure();  // skip SSL verification (testing)

    HTTPClient http;

    http.begin(client, "https://aquacare.my.id/postData/postData.php");
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"suhu\":" + String(temp, 2) + ",";
    json += "\"tds\":" + String(tds, 2) + ",";
    json += "\"ph\":" + String(ph, 2) + ",";
    json += "\"kekeruhan\":" + String(ntu, 2) + ",";
    json += "\"signal\":" + String(percent);
    json += "}";

    PRINT("Kirim JSON: ");
    PRINTLN(json);

    int httpResponseCode = http.POST(json);

    PRINT("HTTP Code: ");
    PRINTLN(httpResponseCode);

    String response = http.getString();
    PRINT("Response: ");
    PRINTLN(response);

    http.end();

  } else {
    PRINTLN("WiFi tidak terhubung!");
  }
}

unsigned long prevMilliPost = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  initWiFi();
  PRINTLN("\nWiFi Connected!");
  PRINTLN("=== ESP32 TCP RECEIVER START ===");
}

void loop() {
  unsigned long currentMills = millis();

  if (currentMills - prevMilliPost >= 2000) {
    prevMilliPost = currentMills;

    waterTemp++;
    tdsValue++;
    phValue++;
    ntuValue++;

    sendToServer(waterTemp, tdsValue, phValue, ntuValue);
  }

  if (waterTemp >= 15) {
    waterTemp = 0;
    tdsValue = 0;
    phValue = 0;
    ntuValue = 0;
  }
}