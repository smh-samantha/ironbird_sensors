#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

void setup() {
  Serial.begin(115200);
  delay(500);

  // Start CAN in loopback mode
  Can1.begin();
  Can1.setBaudRate(1000000);

  // Enable internal loopback
  Can1.enableLoopBack(true);

  Serial.println("CAN loopback test started");
}

void loop() {
  CAN_message_t msg;
  msg.id = 0x321;
  msg.len = 1;
  msg.buf[0] = 0x42;

  Can1.write(msg);
  delay(200);

  CAN_message_t incoming;
  if (Can1.read(incoming)) {
    Serial.print("RX ID: 0x");
    Serial.println(incoming.id, HEX);
    Serial.print("Data[0]: ");
    Serial.println(incoming.buf[0]);
  }
}
