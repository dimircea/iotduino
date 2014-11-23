iotduino
========

Node module for IoT apps/web-apps for PCDuino and compatible devices which can run Node.js.

The module is written using native C/C++ code. Benchmarks with PCDuino v3 board shows that 
it needs 6-7 microseconds per digitalWrite ( ~ 140 - 160 KHz) operation and 4-5 microseconds 
( ~200 - 250 KHz) per digitalRead operation. 

The tests were made by using one million ( 1.000.000) operations then average the execution time.

The code lets room for optimizations, but as it is now is fast enough to use the ultrasonic HC-SR04 distance sensor.



Features:
========
The following Arduino methods are supported:
 * pinMode( pin, state) - set the pin mode to INPUT (0x1), OUTPUT (0x2) or INPUT_PULLUP (0x8)
 * digitalRead( pin)
 * digitalWrite( pin, state) - set the pin state to LOW (0x0) or HIGH ( 0x1) after pinMode was called with OUTPUT as state parameter
 - pulseIn( pin, state, timeout) - read a pulse time for the specified pin and read state
 - micros() - read the number of microseconds ( modulo long type in C) from Epoch.
 - delay( milliseconds) - pause the code execution for the specified number of milliseconds. This method can be improved.
 - delayMicroseconds( microseconds) - pause the code execution for the specified number of microseconds. This method can be improved.


Notes:
========
 - Future plans includes to this library for BeagleBone Black 
