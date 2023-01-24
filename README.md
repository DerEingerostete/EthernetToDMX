<h1 align="center">
  <br>
  <a href="https://github.com/DerEingerostete/EthernetToDMX"><img src="https://raw.githubusercontent.com/DerEingerostete/EthernetToDMX/assets/Logo.png" alt="EthernetToDMX" width="500"></a>
  <br>
   Art-Net Receiver for Arduino Megas 
  <br>
</h1>

___

<p align="center">
  <a href="#features">Features</a> •
  <a href="#hardware">Hardware</a> •
  <a href="#download">Hardware Limitations</a> •
  <a href="#credits">Credits</a> •
  <a href="#license">License</a>
</p>

## Features
 - Sending DMX data via the local network
 - Change the selected universe via a button
 - Shows the current universe on a display
 - Stable connection with good framerate

## Hardware
The following hardware was used:
- Arduino Mega 2560
- Arduino Ethernet Shield
- RS485 to TTL Converter
- Tactical Switch (2 Pin)
- 7 Segment display (10 Pins)
- Cables
- Power Supply

The circuit looks like this:
![Circuit](https://raw.githubusercontent.com/DerEingerostete/EthernetToDMX/assets/Circuit.svg)

## Hardware Limitations
Unfortunately it is not possible to run the project with an Arduino UNO or ESP32, because either the memory is missing or as with the ESP32 the connection is not stable and every now and then the ESP32 crashed.
If someone has found a solution to this problem I would be very happy about a message.

## Credits
Many thanks to the three libraries, without whom I could never have made the project work. 
 - [DmxSimple](https://github.com/PaulStoffregen/DmxSimple)
 - [ArtNet](https://github.com/hideakitai/ArtNet)
 - [EasySevenSeg](https://github.com/NeMaksym/Arduino-EasySevenSeg)

## License
Distributed under the MPL 2.0 License. See [`LICENSE`](/LICENSE) for more information.
