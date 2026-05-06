#define DEBUG true

#if DEBUG
  #define PRINT(x)    Serial.print(x)
  #define PRINTLN(x)  Serial.println(x)
#else
  #define PRINT(x)
  #define PRINTLN(x)
#endif

#include <OneWire.h>
#include <DallasTemperature.h>

// ================= DS18B20 =================
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ================= TDS =================
#define TDS_PIN A1
#define VREF 5.0
#define SCOUNT 30

int analogBuffer[SCOUNT];
int analogBufferIndex = 0;

float waterTemp = 25.0;
float tdsValue = 0;

// ================= FUNCTIONS =================
float readTempWater() {
  sensors.requestTemperatures();
  waterTemp = sensors.getTempCByIndex(0);

  PRINT("Temperature: ");
  PRINT(waterTemp);
  PRINTLN(" °C");

  return waterTemp;
}

void tdsRead() {
  analogBuffer[analogBufferIndex++] = analogRead(TDS_PIN);

  if (analogBufferIndex >= SCOUNT) analogBufferIndex = 0;

  long total = 0;
  for (int i = 0; i < SCOUNT; i++) {
    total += analogBuffer[i];
  }

  float averageADC = total / (float)SCOUNT;
  float averageVoltage = averageADC * VREF / 1024.0;

  float compensationCoefficient = 1.0 + 0.02 * (waterTemp - 25.0);
  float compensationVoltage = averageVoltage / compensationCoefficient;

  tdsValue = (
    133.42 * compensationVoltage * compensationVoltage * compensationVoltage
    - 255.86 * compensationVoltage * compensationVoltage
    + 857.39 * compensationVoltage
  ) * 0.5;

  PRINT("TDS Value: ");
  PRINT(tdsValue);
  PRINTLN(" ppm");
}

// ================= PH METER =================
#define PH_PIN A0
#define VREF 5.0
#define SAMPLE_COUNT 20

float slope25 = -5.66;   // hasil kalibrasi pada 25°C
float offset  = 21.20;    

float readPH(float waterTemp) {
  long total = 0;
  for (int i = 0; i < 20; i++) {
    total += analogRead(PH_PIN);
    delay(10);
  }

  float voltage = (total / 20.0) * 5.0 / 1024.0;
  // Temperature Compensation
  float slopeTemp = slope25 * ((waterTemp + 273.15) / 298.15);
  float phValue = slopeTemp * voltage + offset;
  
  PRINT("PH Value: ");
  PRINTLN(phValue);
  return phValue;
}

// ================= KEKERUHAN AIR METER =================
#define TURBIDITY_PIN A2
#define VREF 5.0

float readTurbidity() {
  long total = 0;

  for (int i = 0; i < 20; i++) {
    total += analogRead(TURBIDITY_PIN);
    delay(10);
  }

  float adc = total / 20.0;
  float voltage = adc * VREF / 1024.0;

  float ntu = -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;

  if (ntu < 0) ntu = 0;
  PRINT("Turbidity Value: ");
  PRINT(ntu);
  PRINTLN(" NTU");

  return ntu;
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  sensors.begin();

  PRINTLN("System Ready");
}

// ================= LOOP =================
void loop() {
  readTempWater();   // baca suhu 1x
  tdsRead();         // pakai suhu tersebut untuk kompensasi
  readPH(waterTemp);
  readTurbidity();
  delay(1000);
}