var duino = require( '../build/Release/iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, pin = 0;
    
// ADC pins to INPUT
for ( pin = pins.A0; pin <= pins.A5; pin++) {
  duino.pinMode( pin, pinMode.INPUT);
}
// repeat every 500 milliseconds, 2 times per second
setInterval( function () { 
  var values = [];
  for ( pin = pins.A0; pin <= pins.A5; pin++) {
    values[pin] = duino.analogRead( pin);
  }
  // read the analogous value
  console.log( values);
}, 500);
