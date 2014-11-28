var duino = require( '/home/ubuntu/WoTCO/node/dev/modules/iotduino/build/Release/iotduino.node');

duino.pinMode( 13, 1);
setInterval( function () { 
  duino.digitalWrite( 13, !duino.digitalRead( 13));
  console.log( "LED is now " + ( duino.digitalRead( 13) === 1 ? "ON" : "OFF"));
}, 1000);