var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, trigPin = pins.GPIO2, echoPin = pins.GPIO3, 
    distanceCm = -1;

// the trigger pin (GPIO2) is set as OUTPUT
duino.pinMode( trigPin, pinMode.OUTPUT);
// the echo pin (GPIO3) is set as INPUT
duino.pinMode( echoPin, pinMode.INPUT);
// read the sensor every 100 milliseconds, 10 times per second
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
}, 100);