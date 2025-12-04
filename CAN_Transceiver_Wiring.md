## CAN Transceiver Wiring Guide (TJA1051)

### Overview
This document explains:
- How to wire the TJA1051 CAN transceiver to the Teensy 4.1
- How to wire the transceiver to the CubePilot CAN port
- Why the transceiver requires 5 V
- How to test CAN communication in Mission Planner
- How to run the Teensy test code

---

## 1. Teensy 4.1 → TJA1051 Wiring

### Teensy CAN1 Pins
- CAN1_TX = Pin **22**
- CAN1_RX = Pin **23**

### TJA1051 Pins
Your board labels may differ, but the common mapping is:

| TJA1051 Pin | Connect To | Notes |
|-------------|------------|-------|
| **TXD**     | Teensy **22** | Teensy → CAN bus |
| **RXD**     | Teensy **23** | CAN bus → Teensy |
| **VCC**     | Teensy **5V** | Required for transceiver power |
| **GND**     | Teensy **GND** | Common ground |
| **STB / SLNT** | **GND** | Must be LOW for normal mode (not standby) |

**Important:**  
The TJA1051 MUST be powered from **5 V**.  
Even though the Teensy is a 3.3 V device, the TJA1051 uses 5 V to generate the correct CAN bus differential voltages. Its TXD/RXD pins are 3.3 V–compatible.

---

## 2. TJA1051 → CubePilot Wiring

The Cube CAN port is a standard 3-wire CAN interface:

| TJA1051 Pin | CubePilot CAN | Purpose |
|-------------|---------------|---------|
| **CANH** | CANH | High line |
| **CANL** | CANL | Low line |
| **GND**  | GND  | Shared ground |

Use twisted pair for CANH/CANL if possible.

---

## 3. CAN Bus Termination

A CAN bus must have **120 Ω** at each end:

- CubePilot CAN port already has termination.
- Add **120 Ω between CANH and CANL** at the Teensy/transceiver end.

Your wiring diagram shows a 120 Ω on the Teensy side — correct.

---

## 4. Teensy Test Code (Minimal CAN Transmit)

Upload this sketch to the Teensy to confirm the transceiver is working:

```cpp
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

void setup() {
  Serial.begin(115200);
  Can1.begin();
  Can1.setBaudRate(1000000);   // Cube uses 1 Mbps
}

void loop() {
  CAN_message_t msg;
  msg.id = 0x123;
  msg.len = 1;
  msg.buf[0] = 0xAA;

  Can1.write(msg);
  delay(200);
}
```

This code continuously sends CAN frames so the Cube can detect the node.

---

## 5. Testing With CubePilot in Mission Planner

1. Connect Teensy → TJA1051 → Cube CAN1.
2. Power the Cube from battery **(USB does NOT power CAN)**.
3. Open **Mission Planner → Config → UAVCAN** or **Status → CAN**.
4. You should see:
   - **CAN1: OK**
   - No incrementing error counters
   - Data frames appear if CAN debugging is enabled

If CAN1 shows errors:
- Check STB/SLNT is grounded  
- Check 120 Ω termination  
- Check TXD = pin 22 and RXD = pin 23  
- Check Cube is not USB-only powered  

---

## 6. Why 5 V Power Is Required

The TJA1051 transceiver implements the ISO 11898-2 CAN physical layer, which drives:
- CANH ≈ 3.5 V
- CANL ≈ 1.5 V

These differential voltages cannot be produced from 3.3 V.  
Thus the chip requires **5 V VCC**, while still safely interfacing with 3.3 V logic.

---

## 7. Summary

- **VCC = 5 V** (must be)
- **STB/SLNT = GND**
- **TXD = 22**, **RXD = 23**
- **CANH/CANL → CubePilot**
- **120 Ω termination at Teensy side**
- Teensy test code verifies bus activity
- Mission Planner confirms Cube sees CAN frames

This setup matches your wiring diagram exactly and will work reliably.


