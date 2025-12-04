#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>
#include <AS5048A.h>
#include <SPI.h>

// -------------------------
// PRESSURE SENSORS (I2C)
// -------------------------

MS5803 sensor1(ADDRESS_HIGH);   // Wire  (SDA=18, SCL=19)
MS5803 sensor2(ADDRESS_HIGH);   // Wire1 (SDA1=17, SCL1=16)

double P_base_1, P_base_2;
double P1, P2;
double dP1, dP2;
float  T1, T2;

// -------------------------
// ENCODER (SPI)
// -------------------------

#define CS1 10
AS5048A enc1(CS1);

// -------------------------
// SETUP
// -------------------------

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // ---- Pressure Sensors ----
  Wire.begin();     // bus 0 SDA/SCL = 18/19
  Wire1.begin();    // bus 1 SDA1/SCL1 = 17/16

  sensor1.reset();
  sensor2.reset();
  
  sensor1.begin(Wire);
  sensor2.begin(Wire1);

  // Baseline pressures
  P_base_1 = sensor1.getPressure(ADC_4096);
  P_base_2 = sensor2.getPressure(ADC_4096);

  // ---- Encoder (SPI) ----
  SPI.begin(); 
  pinMode(CS1, OUTPUT);
  digitalWrite(CS1, HIGH);
  enc1.SPI_setup();
}

// -------------------------
// LOOP
// -------------------------

void loop() {

  // ----- PRESSURE READINGS -----
  T1 = sensor1.getTemperature(CELSIUS, ADC_512);
  T2 = sensor2.getTemperature(CELSIUS, ADC_512);
M
  P1 = sensor1.getPressure(ADC_4096);
  P2 = sensor2.getPressure(ADC_4096);

  dP1 = P1 - P_base_1;
  dP2 = P2 - P_base_2;

  // ----- ENCODER READING -----
  enc1.get_info(5);  // fast update (same as your working sketch)

  int pos = enc1.get_pos();
  float angle = (pos / 16384.0f) * 360.0f;  // EXACT working formula

  // ----- OUTPUT -----
  Serial.print("P1: "); Serial.print(P1);
  Serial.print("  P2: "); Serial.print(P2);

  Serial.print("  dP1: "); Serial.print(dP1);
  Serial.print("  dP2: "); Serial.print(dP2);

  Serial.print("  T1: "); Serial.print(T1);
  Serial.print("  T2: "); Serial.print(T2);

  Serial.print("  |  Angle: ");
  Serial.println(angle, 2);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);  
}