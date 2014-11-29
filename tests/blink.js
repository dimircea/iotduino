var duino = require( 'iotduino')
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, ledPin = pins.GPIO13;
    
// pin 13 (GPIO13) is set as OUTPUT
duino.pinMode( ledPin, pinMode.OUTPUT);
// repeat every 500 milliseconds, 2 times per second
setInterval( function () { 
  // alternate the pin state between HIGH and LOW 
  duino.digitalWrite( ledPin, !duino.digitalRead( ledPin));
}, 500);
