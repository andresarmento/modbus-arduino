Modbus Library for Arduino
==========================

This library allows your Arduino to communicate via Modbus protocol. The Mosbus is a master-slave protocol
used in industrial automation and can be used in other areas, such as home automation.

The Modbus generally uses serial RS-232 or RS-485 as physical layer (then called Modbus Serial) and
TCP/IP via Ethernet or WiFi (Modbus over IP).

In the current version the library allows the Arduino operate as a slave, supporting Modbus Serial and
Modbus over IP. For more information sbout Modbus see:

http://pt.wikipedia.org/wiki/Modbus http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf
http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf

<b>Author's note (motivation and thanks):</b>

It all started when I found the Modbus RTU Arduino library of Juan Pablo Zometa. I had extend the
library to support other Modbus functions.

After researching several other Modbus libraries I realized strengths and weaknesses in all of them.
I also thought it would be cool have a base library for Modbus and derive it for each type of physical layer used.

I appreciate the work of all the authors of the other libraries, of which I used several ideas to compose the modbus-arduino.
At the end of this document is a list of libraries and their authors.

Features
========

<ul>
<li>Operates as a slave (master mode in development) </li>
<li>Supports Modbus Serial (RS-232 or RS485) and Modbus IP (TCP, not keep-alive)</li>
<li>Reply exception messages for all supported functions</li>
<li>Modbus functions supported:</li>
<ul>
    <li>0x01 - Read Coils</li>
    <li>0x02 - Read Input Status (Read Discrete Inputs)</li>
    <li>0x03 - Read Holding Registers</li>
    <li>0x04 - Read Input Registers</li>
    <li>0x05 - Write Single Coil</li>
    <li>0x06 - Write Single Register</li>
    <li>0x0F - Write Multiple Coils</li>
    <li>0x10 - Write Multiple Registers</li>
</ul>
</ul>

<b>Notes:</b>

1. When using Modbus IP the transport protocol is TCP (port 502) and the connection is terminated to each transmitted message, that is, is not a keep-alive type connection.

2. The offsets for registers are 0-based. So be careful when setting your supervisory system or your testing software. For example, in ScadaBR (http://www.scadabr.com.br)
offsets are 0-based, then, a register configured as 100 in the library is set to 100 in ScadaBR. On the other hand, in the CAS Modbus Scanner
(http://www.chipkin.com/products/software/modbus-software/cas-modbus-scanner/) offsets are 1-based, so a register configured as 100 in library should be 101 in this software.

How to
======

There are three classes corresponding to three headers that may be used:

<ul>
<li>Modbus - Base Library</li>
<li>ModbusSerial - Modbus Serial Library (RS-232 and RS-485)</li>
<li>ModbusIP - IP Modbus Library</li>
</ul>

By opting for Modbus Serial or Modbus IP you must include in your sketch the corresponding header and the base library header, eg:

```
#include <Modbus.h>
#include <ModbusSerial.h>
```

<b>Modbus Jargon</b>

In this library was decided to use the terms used in Modbus to the methods names, then is important clarify the names of
register types:

| Register type        | Use as             | Access            | Library methods       |
| -------------------- | ------------------ | ----------------- | --------------------- |
| Coil                 | Digital Output     | Read/Write        | addCoil(), Coil()     |
| Holding Register     | Analog Output      | Read/Write        | addHReg(), HReg()     |
| Input Status         | Digital Input      | Read Only         | addIsts(), Ists()     |
| Input Register       | Analog Input       | Read Only         | addIreg(), Ireg()     |

<b>Notes:</b>

1. <i>Input Status</i> is sometimes called <i>Discrete Input</i>.
2. <i>Holding Register</i> or just <i>Register</i> is also used to store values in the slave.
3. Examples of use: A <i>Coil</i> can be used to drive a lamp or LED. A <i>Holding Register</i> to
store a counter or drive a Servo Motor. A <i>Input Status</i> can be used with a reed switch
in a door sensor and a <i>Input Register</i> with a temperature sensor.


<h3>Modbus Serial</h3>

There are four examples that can be accessed from the Arduino interface, once you have installed the library.
Let's look at the example Lamp.ino (only the parts concerning Modbus will be commented):

```
#include <Modbus.h>
#include <ModbusSerial.h>
```
Inclusion of the necessary libraries.


```
const int LAMP1_COIL = 100;
```
Sets the Modbus register to represent a lamp or LED. This value is the offset (0-based) to be placed in its supervisory or testing software.
Note that if your software uses offsets 1-based the set value there should be 101, for this example.


```
ModbusSerial mb;
```
Create the mb instance (ModbusSerial) to be used.


```
mb.config (& Serial, 38400, SERIAL_8N1);
mb.setSlaveId (10);
```
Sets the serial port and the slave Id. Note that the serial port is passed as reference, which permits the use of other serial ports in other Arduino models.
The bitrate and byte format (8N1) is being set. If you are using RS-485 the configuration of another pin to control transmission/reception is required.
This is done as follows:

```
mb.config (& Serial, 38400, SERIAL_8N1, 2);
```
In this case, the pin 2 will be used to control TX/RX.


```
mb.addCoil (LAMP1_COIL);
```
Adds the register type Coil (digital output) that will be responsible for activating the LED or lamp and verify their status.
The library allows you to set an initial value for the register:

```
mb.addCoil (LAMP1_COIL, true);
```
In this case the register is added and set to true. If you use the first form the default value is false.


```
mb.task ();
```
This method makes all magic, answering requests and changing the registers if necessary, it should be called only once, early in the loop.


```
digitalWrite (ledPin, mb.Coil (LAMP1_COIL));
```
Finally the value of LAMP1_COIL register is used to drive the lamp or LED.


In much the same way, the other examples show the use of other methods available in the library:

```
void addCoil (offset word, bool value)
void addHreg (offset word, word value)
void addIsts (offset word, bool value)
void addIreg (offset word, word value)
```
Adds registers and configures initial value if specified.

```
bool Coil (offset word, bool value)
bool Hreg (offset word, word value)
bool Ist (offset word, bool value)
bool IREG (offset word, word value)
```
Sets a value to the register.

```
bool Coil (offset word)
Hreg word (word offset)
bool Ist (offset word)
IREG word (word offset)
```
Returns the value of a register.


<h3>Modbus IP</h3>

There are four examples that can be accessed from the Arduino interface, once you have installed the library.
Let's look at the example Switch.ino (only the parts concerning Modbus will be commented):

```
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
```
Inclusion of the necessary libraries.

```
const int SWITCH_ISTS = 100;
```
Sets the Modbus register to represent the switch. This value is the offset (0-based) to be placed in its supervisory or testing software.
Note that if your software uses offsets 1-based the set value there should be 101, for this example.

```
ModbusIP mb;
```
Create the mb instance (ModbusIP) to be used.


```
mac byte [] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
ip byte [] = {192, 168, 1, 120};
mb.config (mac, ip);
```

Sets the Ethernet shield. The values ​​of the MAC Address and the IP are passed by the config() method.
The syntax is equal to Arduino Ethernet class, and supports the following formats:

```
void config (uint8_t * mac)
void config (uint8_t * mac, IPAddress ip)
void config (uint8_t * mac, IPAddress ip, IPAddress dns)
void config (uint8_t * mac, IPAddress ip, IPAddress dns, gateway IPAddress)
void config (uint8_t * mac, IPAddress ip, IPAddress dns, IPAddress gateway, subnet IPAddress)
```

Then we have:
```
mb.addIsts (SWITCH_ISTS);
```
Adds the register type Input Status (digital input) that is responsible for detecting if a switch is in state on or off.
The library allows you to set an initial value for the register:

```
mb.addIsts (SWITCH_ISTS, true);
```
In this case the register is added and set to true. If you use the first form the default value is false.


```
mb.task ();
```
This method makes all magic, answering requests and changing the registers if necessary, it should be called only once, early in the loop.


```
mb.Ists (SWITCH_ISTS, digitalRead (switchPin));
```
Finally the value of SWITCH_ISTS register changes as the state of the selected digital input.


Other Modbus libraries
======================

<b>Arduino Modbus RTU</b><br>
Author: Juan Pablo Zometa, Samuel and Marco Andras Tucsni<br>
Year: 2010<br>
Website: https://sites.google.com/site/jpmzometa/

<b>Simple Modbus</b><br>
Author: Bester.J<br>
Year: 2013 Website: https://code.google.com/p/simple-modbus/

<b>Arduino-Modbus slave</b><br>
Jason Vreeland [CodeRage]<br>
Year: 2010<br>
Website: http://code.google.com/p/arduino-modbus-slave/

<b>Mudbus (Modbus TCP)</b><br>
Author: Dee Wykoff<br>
Year: 2011<br>
Website: http://code.google.com/p/mudbus/

<b>ModbusMaster Library for Arduino</b><br>
Author: Doc Walker<br>
Year: 2012<br>
Website: https://github.com/4-20ma/ModbusMaster<br>
Website: http://playground.arduino.cc/Code/ModbusMaster


Contributions
=============
http://github.com/andresarmento/modbus-arduino
prof (at) andresarmento (dot) com

License
=======
The code in this repo is licensed under the BSD New License. See LICENSE.txt for more info.

