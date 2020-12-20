#include <Arduino.h>
#include <Manchester.h>

#define LED_BUILTIN PB1
#define ON          1
#define OFF         0
#define RX_PIN      PB0
#define BUFFER_SIZE 2

uint8_t ledState = ON;
uint8_t buffer[BUFFER_SIZE];

void module_test() {
  digitalWrite(LED_BUILTIN, OFF);
  Serial.println("LED OFF");
  delay(1000);
  digitalWrite(LED_BUILTIN, ON);
  Serial.println("LED ON");
  delay(1000);
  Serial.println("BYE!");
}

void setup() {

  Serial.begin(9600);
  Serial.println("\n***\nReceiver is up. Hey there!\n***\n");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);


  man.setupReceive(RX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() {

  if (man.receiveComplete())
  {
    Serial.println("[DEBUG] Message received");
    uint8_t receivedSize = 0;

    //do something with the data in 'buffer' here before you start receiving to the same buffer again
    receivedSize = buffer[0];
    Serial.println("Received size: " + String(receivedSize));

    for (uint8_t i = 1; i < receivedSize; i++)
      Serial.write(buffer[i]);
    Serial.println();

    man.beginReceiveArray(BUFFER_SIZE, buffer);

    ledState = ++ledState % 2;
    digitalWrite(LED_BUILTIN, ledState);
  }

}
