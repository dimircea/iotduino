IoTDuino
========

Node module for IoT apps/web-apps for PCDuino and compatible devices which can run Node.js.

The module is written using native C/C++ code. Benchmarks with PCDuino v3 board shows that 
it needs 6-7 microseconds per digitalWrite ( ~ 140 - 160 KHz) operation and 4-5 microseconds 
( ~200 - 250 KHz) per digitalRead operation. 

The tests were made by using one million ( 1.000.000) operations then average the execution time.

The code lets room for optimizations, but in its current state is fast enough to be used with HC-SR04 ultrasonic distance sensor.

Features:
========
The following Arduino methods are supported:
 * `pinMode( pin, state)` - set the pin mode to INPUT (0x1), OUTPUT (0x2) or INPUT_PULLUP (0x8)
 * `digitalRead( pin)` - read the current state of the selected pin.
 * `digitalWrite( pin, state)` - set the pin state to LOW (0x0) or HIGH ( 0x1) after pinMode was called with OUTPUT as state parameter
 - `pulseIn( pin, state, timeout)` - read a pulse time for the specified pin and read state
 - `micros()` - read the number of microseconds ( modulo long type in C) from Epoch.
 - `delay( milliseconds)` - pause the code execution for the specified number of milliseconds. This method can be improved.
 - `delayMicroseconds( microseconds)` - pause the code execution for the specified number of microseconds. This method can be improved.
 
Install:
========
Use `npm` like for any other `node` module:
```
npm install iotduino
```
for local installations (only for the application folder from which you execute the command), or use:

```
npm install -g iotduino
```
for a global installation (available for any node application). 
For the global installation case, don't forgot to use `sudo` in case you are not `root` on a linux system when you try to install the module.
 
Usage:
========
The well known blink example for the built-in PCDuino v3 LED connected to pin 13 ( same as for Arduino UNO and alike):
```
var duino = require( 'iotduino');

duino.pinMode( 13, 1);
setInterval( function () { 
  duino.digitalWrite( 13, !duino.digitalRead( 13));
}, 500);
```

Reading distance from the HC-SR04 ultrasonic senosor ( note that it requires voltage level shifter from 5V to 3.3V!):
```
var duino = require( 'iotduino'),
    trigPin = 2, echoPin = 3, cm = -1;
    
duino.pinMode( trigPin, 1);
duino.pinMode( echoPin, 0);

setInterval ( function () { 
  duino.digitalWrite( trigPin, 0);
  duino.delayMicroseconds( 2);
  duino.digitalWrite( trigPin, 1);
  duino.delayMicroseconds( 10);
  duino.digitalWrite( trigPin, 0);
  cm = duino.pulseIn( echoPin, 1, 100000) / 58.0; 
  console.log( "Distance: " + cm.toFixed(2) + " cm");
}, 333);
```

Notes:
========
 - Improvements and are made on a weekly basis, therefore check for the latest updates every week.
 - Future plans includes to port the library to BeagleBone White/Black and possible to other similar boards.
