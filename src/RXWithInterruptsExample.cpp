// #include <Arduino.h>

// #define LED_BUILTIN 1

// //NOTE: The ATtiny85 must be set to use the 8-MHz internal clock
// //      This requires "Burn Bootloader" to be run first

// // Receives commands from an Etekcity RF outlet remote
// // Must know the specific codes being transmitted
// // 0 bit: 188us
// // 1 bit: 3x zero bit length
// // Sync (start) time: 30x zero bit
// // Message length: 24 data bits, 1 stop bit
// // Written by Boomer48: September 2019

// #define PULSE_IN 0
// #define Msg_Length 3 // Defined message length for specific transmitter

// // All times in microseconds
// #define Min_Gap_Time 5000 // nominal = 5600
// #define Max_Gap_Time 6200 // nominal = 5600
// #define One_Bit_Time 120 // nominal = 188
// #define Zero_Bit_Time 500 // nominal = 564

// // Defines for expected on/off commands
// #define RF_On_1 0x44
// #define RF_On_2 0x15
// #define RF_On_3 0x33 // on
// #define RF_Off_1 0x44
// #define RF_Off_2 0x15
// #define RF_Off_3 0x3C // off

// uint8_t RF_Bit_Count;
// uint8_t RF_Byte_Count;
// uint8_t RF_Byte = 0;
// uint8_t RF_Message[] = {0, 0, 0}; // 24 bits
// unsigned long Start_Time = 0;
// uint32_t Pulse_Width;
// uint8_t Started = false;

// /**********
//  *  MAIN  *
//  **********/

// void setup() {

//   // For Digispark ATTiny85 TinyDebugSerial overrides Serial and uses PB3 pin as TX
//   Serial.begin(9600);
//   Serial.println("\n******* Hello! Board is up. *******");

//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, LOW);

//   digitalWrite(PULSE_IN, HIGH); //turn on pullup

//   // Set interrupt-on-change registers
//   bitClear(GIMSK, INT0); // disable INT0 external interrupt
//   bitSet(GIMSK, PCIE); // enable interrupt-on-change
//   bitSet(PCMSK, PULSE_IN);
// }

// void loop() {
//   // uint8_t i;
//   String msg;

//   if (RF_Byte_Count == Msg_Length) {

//     noInterrupts();

//     // if ((RF_Message[0] == RF_On_1) && (RF_Message[1] == RF_On_2) && (RF_Message[2] == RF_On_3))
//     //   digitalWrite(LED_BUILTIN, 1);
//     // else if ((RF_Message[0] == RF_Off_1) && (RF_Message[1] == RF_Off_2) && (RF_Message[2] == RF_Off_3))
//     //   digitalWrite(LED_BUILTIN, 0);

//     for (uint8_t i: RF_Message) {
//       digitalWrite(LED_BUILTIN, 1);
//       Serial.println(String(i));
//       digitalWrite(LED_BUILTIN, 0);
//     }


//     Started = false;
//     interrupts();
//   }
// }

// // Interrupt-on-change handler
// ISR (PCINT0_vect)
// {
//   //when the pin goes LOW record the pulse start time
//   if (digitalRead(PULSE_IN) == LOW) {
//     Start_Time = micros();
//   }
//   else // pin went high
//     if (Start_Time != 0) {
//       //calculate the pulse width
//       Pulse_Width = ((volatile int)micros() - Start_Time);
//       //clear the timer
//       Start_Time = 0;

//       // Now check for valid message
//       if (Pulse_Width < Max_Gap_Time) {
//         if (Started == true) {
//           if (Pulse_Width > Zero_Bit_Time) {
//             // bit = 0
//             RF_Bit_Count--; // bits received MSB first
//             bitClear(RF_Message[RF_Byte_Count], RF_Bit_Count);
//             if (RF_Bit_Count == 0) {
//               RF_Byte_Count++;
//               RF_Bit_Count = 8;
//             }
//           }
//           else if (Pulse_Width > One_Bit_Time) {
//             // bit = 1
//             RF_Bit_Count--; // bits received MSB first
//             bitSet(RF_Message[RF_Byte_Count], RF_Bit_Count);
//             if (RF_Bit_Count == 0) {
//               RF_Byte_Count++;
//               RF_Bit_Count = 8;
//             }
//           }
//           else { // bad bit
//             Started = false;
//           }
//         } // Started

//         else if (Pulse_Width > Min_Gap_Time) {
//           // start of message
//           Started = true;
//           RF_Byte_Count = 0;
//           RF_Bit_Count = 8;
//         }
//       }
//       else { // invalid message
//         Started = false;
//       }
//     }
// }
