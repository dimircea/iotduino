/* WORK IN PROGRESS: not ready yet! */


var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, dataPin = pins.GPIO4;
    
// repeat every 2000 milliseconds, once every 2 seconds
// DHT22 sensor can only be read at intervals greater than 2 seconds
setInterval( function () { 
  // stores the index of the current read bit (out of 40)
  var data = [], totalBitsNr = 40, bitNr = 0, 
      signalTime = 0, startTime = 0,
      temperature = 0, humidity = 0;
  // pin 4 (GPIO4) is set as OUTPUT
  duino.pinMode( dataPin, pinMode.OUTPUT);
  // send LOW signal for 1 millisecond
  // this informs the sensor to give back data
  duino.digitalWrite( dataPin, pinState.LOW);
  // keep the data pin low for about 1 millisecond
  duino.delay(1);
  // allow the sensor to set the pin HIGH
  duino.digitalWrite( dataPin, pinState.HIGH);
  // the data pin is set as INPUT for data reading
  duino.pinMode( dataPin, pinMode.INPUT);
  // As a confirmation of receiving the command, 
  // the sensor sends a HIGH signal for 20-40 microseconds
  // then a LOW signal for about 80 microseconds 
  // and then a HIGH signal for about 80 microseconds
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.HIGH) {
    if ( duino.micros() - startTime > 50) {
      console.log (" Error 1");
      return;
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.LOW) {
    if ( duino.micros() - startTime > 90) {
      console.log (" Error 2");
      return;
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.HIGH) {
    if ( duino.micros() - startTime > 90) {
      console.log (" Error 3");
      return;
    }
  }
  // read the 40 bits of data sent by the sensor
  for ( bitNr = 0; bitNr < totalBitsNr; bitNr++) {
    // receive the about 50 microseconds LOW signal, 
    // used as separator before two consecutive data bits
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.LOW) {
      if ( duino.micros() - startTime > 90) {
        console.log (" Error 4");
        return;
      }
    }
    // receive the data bit
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.HIGH) {
      if ( duino.micros() - startTime > 120) {
        console.log (" Error 5");
        return;
      }
    }
    signalTime = duino.micros() - startTime;
    // signal is between 20-40 microseconds, then it is a '0' bit
    // signal is between 60-80 microseconds, then it is a '1' bit
    data[bitNr] = signalTime > 50 ? 1 : 0;
  }
  
  for ( bitNr = 15; bitNr >= 0; bitNr--) {
    humidity += data[bitNr] * Math.pow( 2, 15 - bitNr);
    temperature += data[bitNr + 16] * Math.pow( 2, 15 - bitNr);
  }
  humidity /= 10.0;
  temperature /= 10.0;
  
  console.log( "Temperature: " + temperature + "°C, Humidity: " + humidity + "%");  
}, 2000);
