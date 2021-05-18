IoT Radio Receiver Test
=======================

433MHz/315MHz receiver test with the PlatformIO and Arduino framework

Boards:
* Arduino UNO
* Digispark ATTiny85 USB clone (LED on PB1)
  Tested with non-default CPU frequencies 1, 8 and 16MHz. Default 16.5MHz feels to be the most stable due to being calibrated precisely by the micronucleus,
  while 1MHz has the most unstable receiving which probably could be fixed by adding antenna to the receiver module or otherwise by oscillator calibration.

Receivers:
* MX-RM-5V 433MHz ASK OOK super regenerative
* WL101-341 433MHz superheterodyne

Tips to improve your transmit/receive distance: Attaching an antenna will greatly improve transmit distance (from few cm to few m) for 315Mhz use 23.81 cm straight wire, for 433Mhz use 17.28cm straight wire as antenna. If possible keep the wire straight, away from ground wire, and away from the rest of the circuit and battery pack. Transmitter can use anything from 3.3V to 12V. Increasing voltage will increase transmit distance. If you are using voltage regulator, attach transmitter directly to the battery. Receiver needs 5V, it doesn't work on 3.3V


Development
-----------

### UART (Serial)

#### Digispark ATTiny85

[Digistump dtiny core](https://github.com/ArminJo/DigistumpArduino) `TinyDebugSerial` overrides `Serial` and uses:
- `PB3` as TX for CPU frequencies <= 8MHz
- `PB2` as TX for other frequencies

See [TinyDebugSerial.h](https://github.com/ArminJo/DigistumpArduino/blob/master/digistump-avr/cores/tiny/TinyDebugSerial.h) for the details

#### Generic ATtiny85

[SpenceKonde tiny core](https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_x5.md#uart-serial-support) `TinySoftwareSerial` overrides `Serial` and uses:
- `AIN0` (`PB0`) as TX
- `AIN1` (`PB1`) as RX


TODO
----

Evaluate https://mchr3k.github.io/arduino-libs-manchester/ library for data transmission with 315/433MHz transmitter/receiver
