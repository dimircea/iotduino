IoTDuino
========

Node module for IoT apps/web-apps for PCDuino and compatible devices which can run Node.js.

The module is written using native C/C++ code. Benchmarks with PCDuino v3 board shows that 
it needs 6-7 microseconds per `digitalWrite` ( ~ 140 - 160 KHz) operation and 4-5 microseconds 
( ~200 - 250 KHz) per `digitalRead` operation. 

The tests were made by using one million ( 1.000.000) read/write operations then average the execution time. Check (or run) the `tests/performance_tests.js` file.

The code lets room for optimizations, but in its current state is fast enough to be used with HC-SR04 ultrasonic distance sensor. 
Work in progress: use the module with `DHT22` humidity and temperature sensor which requires a custom `1-Wire` communication protocol and it is very timming sensitive. 

Features:
========
The following Arduino methods are supported:
 * `pinMode( pin, state)` - set the pin mode to `INPUT` (0x1), `OUTPUT` (0x2) or `INPUT_PULLUP` (0x8)
 * `digitalRead( pin)` - read the current state of the selected pin.
 * `digitalWrite( pin, state)` - set the pin state to `LOW` (0x0) or `HIGH` ( 0x1) after `pinMode` was called with `OUTPUT` as state parameter
 - `pulseIn( pin, state, timeout)` - read a pulse time for the specified pin and state
 - `micros()` - read the number of microseconds ( modulo `long type` in C) from Epoch.
 - `delay( milliseconds)` - pause the code execution for the specified number of milliseconds. This method implementation is on the way to be improved.
 - `delayMicroseconds( microseconds)` - pause the code execution for the specified number of microseconds. This method implementation is on the way to be improved.
 
Install:
========
Local installation (used for the application on which folder you execute the command):
```
npm install iotduino
```

Global installation (available for any node application):
```
npm install -g iotduino
```
In this case, use `sudo` or login as `root` on a linux when installing the module.
 
Usage:
========
The well known LED blink example by using the built-in PCDuino v3 LED connected to pin 13 ( same as for Arduino UNO and alike):
```
var duino = require( 'iotduino');

// pin 13 is set as OUTPUT
duino.pinMode( 13, 1);
setInterval( function () { 
  // alternate the pin state between HIGH and LOW 
  duino.digitalWrite( 13, !duino.digitalRead( 13));
}, 500);
```

Reading distance with the `HC-SR04` ultrasonic senosor (it requires voltage level shifter from 5V to 3.3V and back since the PCDuino GPIOs are not 5V tolerant and the sensor does now work if powered from 3.3V):
```
var duino = require( 'iotduino'),
    trigPin = 2, echoPin = 3, cm = -1;
    
// pin 2, the aka trigger pin, is set as OUTPUT
duino.pinMode( trigPin, 1);
// pin 3, aka the echo pin, is set as INPUT
duino.pinMode( echoPin, 0);

setInterval ( function () { 
  // the sensor receives LOW at the trigger pin, 
  // to prepare it for data reading
  duino.digitalWrite( trigPin, 0);
  // wait for the sensor to get ready
  duino.delayMicroseconds( 2);
  // inform the sensor that we want to make a reading
  duino.digitalWrite( trigPin, 1);
  duino.delayMicroseconds( 10);
  // put the sensor back in the waiting state
  duino.digitalWrite( trigPin, 0);
  // read the value HIGH time period from the sensor
  // and compute the distance based on it and the physical laws
  cm = duino.pulseIn( echoPin, 1, 100000) / 58.0; 
  // show the distance value in the console
  console.log( "Distance: " + cm.toFixed(2) + " cm");
}, 333);
```

Notes:
========
 - Improvements (and new features) are on the way, so you may want to check for updates from time to time.
 - Future plans includes to port this module to BeagleBone White/Black and possible to other similar boards.
