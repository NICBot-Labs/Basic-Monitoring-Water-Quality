#define PH_PIN A0
#define SAMPLE_COUNT 10

// ===========================
// DATA KALIBRASI 2 TITIK
// GANTI DENGAN HASIL UKUR ANDA
// ===========================
float voltage1 = 3.288;   // contoh tegangan air mineral
float ph1      = 7.0;

float voltage2 = 2.99;   // contoh tegangan air deterjen
float ph2      = 9.0;

// ===========================
// HASIL KALIBRASI
// ===========================
float slope;
float intercept;

// ===========================
void calculateCalibration() {
  slope = (ph2 - ph1) / (voltage2 - voltage1);
  intercept = ph1 - (slope * voltage1);
}

// ===========================
float readPHVoltage() {
  long total = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    total += analogRead(PH_PIN);
    delay(10);
  }

  float adc = total / (float)SAMPLE_COUNT;
  return adc * 5.0 / 1023.0;
}

// ===========================
float readPH() {
  float voltage = readPHVoltage();
  return (slope * voltage) + intercept;
}

// ===========================
void setup() {
  Serial.begin(9600);

  calculateCalibration();

  Serial.println("=== KALIBRASI 2 TITIK ===");
  Serial.print("Slope     : ");
  Serial.println(slope, 6);

  Serial.print("Intercept : ");
  Serial.println(intercept, 6);
}

// ===========================
void loop() {
  float voltage = readPHVoltage();
  float phValue = readPH();

  Serial.print("Voltage : ");
  Serial.print(voltage, 3);

  Serial.print(" V | pH : ");
  Serial.println(phValue, 2);

  delay(1000);
}