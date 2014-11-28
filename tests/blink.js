var duino = require( 'iotduino');

// pin 13 is set as OUTPUT
duino.pinMode( 13, 1);
// repeat every 500 milliseconds, 2 times per second
setInterval( function () { 
  // alternate the pin state between HIGH and LOW 
  duino.digitalWrite( 13, !duino.digitalRead( 13));
}, 500);