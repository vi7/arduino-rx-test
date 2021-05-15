//NOTE: The ATtiny85 must be set to use the 8-MHz internal clock
//      This requires "Burn Bootloader" to be run first

// Receives commands from an Etekcity RF outlet remote
// Must know the specific codes being transmitted
// 0 bit: 188us
// 1 bit: 3x zero bit length
// Sync (start) time: 30x zero bit
// Message length: 24 data bits, 1 stop bit
// Written by Boomer48: September 2019

#include <Arduino.h>

#define Pulse_In 1
#define RX_LED 0 // Shows on/off state for received commands
#define Msg_Length 3 // Defined message length for specific transmitter

// All times in microseconds
#define Min_Gap_Time 5000 // nominal = 5600
#define Max_Gap_Time 6200 // nominal = 5600
#define One_Bit_Time 120 // nominal = 188
#define Zero_Bit_Time 500 // nominal = 564

// Defines for expected on/off commands
#define RF_On_1 0x44
#define RF_On_2 0x15
#define RF_On_3 0x33 // on
#define RF_Off_1 0x44
#define RF_Off_2 0x15
#define RF_Off_3 0x3C // off

byte RF_Bit_Count;
byte RF_Byte_Count;
byte RF_Byte = 0;
byte RF_Message[] = {0, 0, 0}; // 24 bits
unsigned long Start_Time = 0;
int Pulse_Width;
byte Started = false;

void setup() {
  pinMode(RX_LED, OUTPUT);
  digitalWrite(RX_LED, 0);
  digitalWrite(Pulse_In, HIGH); //turn on pullup

  // Set interrupt-on-change registers
  bitClear(GIMSK, INT0); // disable INT0 external interrupt
  bitSet(GIMSK, PCIE); // enable interrupt-on-change
  bitSet(PCMSK, Pulse_In);
}

void loop() {
  byte i;

  if (RF_Byte_Count == Msg_Length) {
    noInterrupts();

    if ((RF_Message[0] == RF_On_1) && (RF_Message[1] == RF_On_2) && (RF_Message[2] == RF_On_3))
      digitalWrite(RX_LED, 1);
    else if ((RF_Message[0] == RF_Off_1) && (RF_Message[1] == RF_Off_2) && (RF_Message[2] == RF_Off_3))
      digitalWrite(RX_LED, 0);

    Started = false;
    interrupts();
  }
} // end loop

// Interrupt-on-change handler
ISR (PCINT0_vect)
{
  //when the pin goes LOW record the pulse start time
  if (digitalRead(Pulse_In) == LOW) {
    Start_Time = micros();
  }
  else // pin went high
    if (Start_Time != 0) {
      //calculate the pulse width
      Pulse_Width = ((volatile int)micros() - Start_Time);
      //clear the timer
      Start_Time = 0;

      // Now check for valid message
      if (Pulse_Width < Max_Gap_Time) {
        if (Started == true) {
          if (Pulse_Width > Zero_Bit_Time) {
            // bit = 0
            RF_Bit_Count--; // bits received MSB first
            bitClear(RF_Message[RF_Byte_Count], RF_Bit_Count);
            if (RF_Bit_Count == 0) {
              RF_Byte_Count++;
              RF_Bit_Count = 8;
            }
          }
          else if (Pulse_Width > One_Bit_Time) {
            // bit = 1
            RF_Bit_Count--; // bits received MSB first
            bitSet(RF_Message[RF_Byte_Count], RF_Bit_Count);
            if (RF_Bit_Count == 0) {
              RF_Byte_Count++;
              RF_Bit_Count = 8;
            }
          }
          else { // bad bit
            Started = false;
          }
        } // Started

        else if (Pulse_Width > Min_Gap_Time) {
          // start of message
          Started = true;
          RF_Byte_Count = 0;
          RF_Bit_Count = 8;
        }
      }
      else { // invalid message
        Started = false;
      }
    }
}
