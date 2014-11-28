var duino = require( 'iotduino'),
    trigPin = 2, echoPin = 3, cm = -1;

// pin 2, the aka trigger pin, is set as OUTPUT
duino.pinMode( trigPin, 1);
// pin 3, aka the echo pin, is set as INPUT
duino.pinMode( echoPin, 0);
// read the sensor every 333 milliseconds, ~3 times per second
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