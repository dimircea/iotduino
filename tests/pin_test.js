var duino = require( '../build/Release/iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, pin = pins.GPIO7;
    
duino.pinMode( pin, pinMode.INPUT);
//duino.pinMode( pin, pinMode.OUTPUT);