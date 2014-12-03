var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, pwmPin = pins.GPIO5, value = 0;
    
duino.pinMode( pwmPin, pinMode.OUTPUT);
duino.setPwmFrequency( pwmPin, 781);
setInterval( function () {
  console.log( duino.analogWrite( pwmPin, value));
  console.log( "Activated value: " + value);
  value++;
}, 2000);