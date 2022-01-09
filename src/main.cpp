/*
 * 433MHz/315MHz receiver test with the Arduino framework
 */

#include <Arduino.h>
#include <RCSwitch.h>

#if defined(__AVR_ATtinyX5__)
  #define RX_PIN PB0
  #define RX_LED PB1  // Shows on/off state for received commands
#elif defined(ESP8266)
  #define RX_PIN 4
  #define RX_LED LED_BUILTIN  // Shows on/off state for received commands
#else
  #define RX_PIN 2
  #define RX_LED LED_BUILTIN  // Shows on/off state for received commands
#endif

RCSwitch receiver = RCSwitch();

#if defined(__AVR_ATtinyX5__)
void setupInterrupts() {
  noInterrupts();
  bitClear(GIMSK, INT0);  // disable INT0 external interrupt
  bitSet(GIMSK, PCIE);    // enable interrupt-on-change
  bitSet(PCMSK, RX_PIN);  // activate interrupt for the specific pin
  interrupts();
}
#endif

void setupPins() {
  pinMode(RX_LED, OUTPUT);
  // liveness check by blinking the LED
  digitalWrite(RX_LED, HIGH);
  delay(200);
  digitalWrite(RX_LED, LOW);
}

/*
 * SETUP
 */
void setup() {
  Serial.begin(9600);

#if defined(__AVR_ATtinyX5__)
#if defined(TinySoftwareSerial_h)
  // Disable serial RX to free up PB1 (AIN1) pin
  // https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_x5.md#uart-serial-support
  ACSR &= ~(1 << ACIE);
  ACSR |= ~(1 << ACD);
#endif
  Serial.printf("\n[INFO] Digispark USB is up. Hey there!\n");
#else
  Serial.print("\n[INFO] Arduino UNO is up. Hey there!\n");
#endif

  setupPins();

#if defined(__AVR_ATtinyX5__)
  setupInterrupts();
  // receiver.enableReceive(0);  // Using ATTiny85 INT0 (PB2)
#else
  receiver.enableReceive(digitalPinToInterrupt(RX_PIN));
#endif
}

/*
 * LOOP
 */
void loop() {
  if (receiver.available()) {
    unsigned int length = receiver.getReceivedBitlength();
    digitalWrite(RX_LED, HIGH);
    Serial.print("Received 0x");
    Serial.print(receiver.getReceivedValue(), HEX);
    Serial.print(" / ");
    Serial.print(length);
    Serial.print("bit ");
    Serial.print(" PulseLength: ");
    Serial.print(receiver.getReceivedDelay());
    Serial.print(" microseconds");
    Serial.print(" Protocol: ");
    Serial.println(receiver.getReceivedProtocol());
    Serial.print("Raw data: ");
    for (unsigned int i=0; i<= length*2; i++) {
      Serial.print(receiver.getReceivedRawdata()[i]);
      Serial.print(",");
    }
    Serial.println();
    receiver.resetAvailable();
    delay(50);
  } else {
    digitalWrite(RX_LED, LOW);
  }
}

#if defined(__AVR_ATtinyX5__)
/*
 * Interrupt handler
 */
ISR(PCINT0_vect) { RCSwitch::handleInterrupt(); }
#endif
