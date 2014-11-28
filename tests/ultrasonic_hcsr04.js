var duino = require( '/home/ubuntu/WoTCO/node/dev/modules/iotduino/build/Release/iotduino.node'),
    trigPin = 2, echoPin = 3, cm = -1;
    
duino.pinMode( trigPin, 1);
duino.pinMode( echoPin, 0);

setInterval ( function () { 
  duino.digitalWrite( trigPin, 0);
  duino.delayMicroseconds( 2);
  duino.digitalWrite( trigPin, 1);
  duino.delayMicroseconds( 10);
  duino.digitalWrite( trigPin, 0);
  cm = duino.pulseIn( echoPin, 1, 100000) / 58.0; 
  console.log( "Distance: " + cm.toFixed(2) + " cm");
}, 333);