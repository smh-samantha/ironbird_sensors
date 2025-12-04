#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire2.begin();   // your bus on pins 24/25

  while (true) {
    Serial.println("Trying to initialize BME280...");

    if (bme.begin(0x77, &Wire2)) {      // use your detected address
      Serial.println("BME280 initialised!");
      break;
    }

    Serial.println("BME280 not found. Retrying...");
    delay(1000);   // 1 second retry
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity    = bme.readHumidity();
  float pressure    = bme.readPressure() / 100.0F;

  Serial.print("T = "); Serial.println(temperature);
  Serial.print("H = "); Serial.println(humidity);
  Serial.print("P = "); Serial.println(pressure);

  Serial.println();
  delay(1000);
}
