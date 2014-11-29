IoTDuino
========

Node module for IoT apps/web-apps created by using the PCDuino or other PCDuino compatible devices able to run `Node.js`.

The module uses native C/C++ code. Benchmarks with PCDuino v3 board reveals that a `digitalWrite` operation takes 6-7 microseconds (~ 140 - 160 KHz) and a `digitalRead` operation takes 4-5 microseconds (~200 - 250 KHz).

The tests were made by using one million (1.000.000) read/write operations. Run the `tests/performance_tests.js` script if you like to check the performance of your own board. The results may by different from board to board and also depends on the load of your CPU when the tests are executed (background processes, crons, services, applications, etc).

The code lets room for many optimizations. As it is now, can be used to read the HC-SR04 ultrasonic distance sensor without visible lags. The only limitation is the sensor itself and the measured distance (remember: the speed of the sound in air is approx. 343m/s, which means that one millisecond is required to measure every 34.3cm).

Work in progress: use the module with `DHT22` humidity and temperature sensor which requires a custom `1-Wire` communication protocol (the communication with this sensor is really time sensitive). 

Features:
========
The following Arduino methods are supported:
 * `pinMode( pin, state)` - set the pin mode to `INPUT` (0x1), `OUTPUT` (0x2) or `INPUT_PULLUP` (0x8)
 * `digitalRead( pin)` - read the current state of the selected pin.
 * `digitalWrite( pin, state)` - set the pin state to `LOW` (0x0) or `HIGH` ( 0x1) after `pinMode` was called with `OUTPUT` as state parameter
 * `pulseIn( pin, state, timeout)` - read a pulse time for the specified pin and state
 * `micros()` - read the number of microseconds ( modulo `unsigned long type` in C/C++) elapsed from Epoch (01.01.1970, 00:00:00 UTC).
 * `delay( milliseconds)` - pause the code execution for the specified number of milliseconds. This method implementation is on the way to be improved.
 * `delayMicroseconds( microseconds)` - pause the code execution for the specified number of microseconds. This method implementation is on the way to be improved.

The following constant groups are defined (check the USAGE section for concret examples) :
 * `PinMode` - defines the pin mode states. Possible values: INPUT, OUTPUT and INPUT_PULLUP. Alternative: use 0, 1 and 8.
 * `PinState` - define the possible pin states. Possible values: LOW and HIGH. Alternative: use 0 and 1.
 * `Pins` - define the existing pins. Possible values: GPIO0, GPIO1, ... GPIO23, A0, A1, A2, A3, A4, A5. Alternative: use 0, 1, ..., 23 to identify GPIOs and 0, 1, 2, 3, 4, 5 to identify the analog pins.
 
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
For the global installation case, you have to use `sudo` or to login as `root` on linux systems to install this module.

In case you get the following error:
```
Error: Cannot find module 'iotduino'
```
check the `NODE_PATH` variable (use `echo $NODE_MODULES`) to verify if it points to the right location (normally this is `/usr/local/lib/node_modules`). You may also what to read the [node modules official page](http://nodejs.org/api/modules.html#modules_loading_from_the_global_folders) for additional information. 
Usage:
========
The well known LED blink example by using the built-in PCDuino v3 LED connected to pin 13 ( same as for Arduino UNO and alike):
```
var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, ledPin = pins.GPIO13;
    
// pin 13 (GPIO13) is set as OUTPUT
duino.pinMode( ledPin, pinMode.OUTPUT);
// repeat every 500 milliseconds, 2 times per second
setInterval( function () { 
  // alternate the pin state between HIGH and LOW 
  duino.digitalWrite( ledPin, !duino.digitalRead( ledPin));
}, 500);
```

Reading distance with the `HC-SR04` ultrasonic sensor (requires hardware voltage level shifter from 5V to 3.3V and back since the PCDuino GPIOs are not 5V tolerant and the `HCSR-04` sensor does not work if powered from 3.3V):
```
var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, trigPin = pins.GPIO2, echoPin = pins.GPIO3, 
    distanceCm = -1;

// the trigger pin (GPIO2) is set as OUTPUT
duino.pinMode( trigPin, pinMode.OUTPUT);
// the echo pin (GPIO3) is set as INPUT
duino.pinMode( echoPin, pinMode.INPUT);
// read the sensor every 333 milliseconds, ~3 times per second
setInterval ( function () { 
  // the sensor receives LOW at the trigger pin, 
  // to prepare it for data reading
  duino.digitalWrite( trigPin, pinState.LOW);
  // wait for the sensor to get ready
  duino.delayMicroseconds( 2);
  // inform the sensor that we want to make a reading
  duino.digitalWrite( trigPin, pinState.HIGH);
  duino.delayMicroseconds( 10);
  // end the commands chain to the sensor
  duino.digitalWrite( trigPin, pinState.LOW);
  // read the value HIGH time period from the sensor
  // and compute the distance based on it and the physical laws
  distanceCm = duino.pulseIn( echoPin, pinState.HIGH, 100000) / 58.0; 
  // show the distance value in the console
  console.log( "Distance: " + distanceCm.toFixed(2) + " cm");
}, 333);
```

Work in progress:
========
- add support for `analogRead` and `analogWrite`
- add PWM support

Notes:
========
 - Improvements (and new features) are on the way, you may want to check for updates from time to time.
 - Future plans includes to port this module for BeagleBone White/Black and possible to other similar boards.
