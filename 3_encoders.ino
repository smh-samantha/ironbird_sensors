#include <SPI.h>

// Chip-select pins
#define CS1 10
#define CS2 9
#define CS3 8


uint16_t readRaw(int csPin) {
  digitalWrite(csPin, LOW);
  delayMicroseconds(3);

  // CLOCK OUT DATA using 0xFF 
  uint16_t hi = SPI.transfer(0xFF) & 0x3F;   
  uint16_t lo = SPI.transfer(0xFF);
  uint16_t raw = (hi << 8) | lo;

  digitalWrite(csPin, HIGH);
  return raw;
}

void setup() {
  Serial.begin(115200);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV8);   


  pinMode(CS1, OUTPUT); digitalWrite(CS1, HIGH);
  pinMode(CS2, OUTPUT); digitalWrite(CS2, HIGH);
  pinMode(CS3, OUTPUT); digitalWrite(CS3, HIGH);

  Serial.println("AS5048A RAW + ANGLE (Test-Mode SPI Settings)");
}

void loop() {

  uint16_t rawA = readRaw(CS1);
  uint16_t rawB = readRaw(CS2);
  uint16_t rawC = readRaw(CS3);


  float degA = rawA * 360.0 / 16384.0;
  float degB = rawB * 360.0 / 16384.0;
  float degC = rawC * 360.0 / 16384.0;

  Serial.print("A: "); Serial.print(rawA);
  Serial.print(" ("); Serial.print(degA, 2); Serial.print("°)    ");

  Serial.print("B: "); Serial.print(rawB);
  Serial.print(" ("); Serial.print(degB, 2); Serial.print("°)    ");

  Serial.print("C: "); Serial.print(rawC);
  Serial.print(" ("); Serial.print(degC, 2); Serial.println("°)");

  delay(200);
}
