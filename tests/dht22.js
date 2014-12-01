var duino = require( 'iotduino'),
    pinMode = duino.PinMode, pinState = duino.PinState,
    pins = duino.Pins, dataPin = pins.GPIO4;
    
function readSensor( callback) {
  var data = [], bitGroup = [], checksum = 0,
      totalBitsNr = 40, bitNr = 0, 
      signalTime = 0, startTime = 0,
      temperature = 0, humidity = 0;
      
  // the data pin is set as OUTPUT
  duino.pinMode( dataPin, pinMode.OUTPUT);
  // send LOW signal for 1 millisecond
  // this informs the sensor to give back data
  duino.digitalWrite( dataPin, pinState.LOW);
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
      return readSensor( callback);
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.LOW) {
    if ( duino.micros() - startTime > 90) {
      return readSensor( callback);
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.HIGH) {
    if ( duino.micros() - startTime > 90) {
      return readSensor( callback);
    }
  }
  // read the 40 bits of data sent by the sensor
  for ( bitNr = 0; bitNr < totalBitsNr; bitNr++) {
    // receive the about 50 microseconds LOW signal, 
    // used as separator before two consecutive data bits
    // NOTE: allow +50 microseconds for timeout
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.LOW) {
      if ( duino.micros() - startTime > 100) {
        return readSensor( callback);
      }
    }
    // receive the data bit ( 20 to 70 microseconds)
    // NOTE: allow +50 microseconds for timeout
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.HIGH) {
      if ( duino.micros() - startTime > 120) {
        return readSensor( callback);
      }
    }
    signalTime = duino.micros() - startTime;
    // signal is between 20-40 microseconds, then it is a '0' bit
    // signal is between 60-80 microseconds, then it is a '1' bit
    // NOTE: consider 50 microseconds as the range limit between '0' and '1' bits
    data[bitNr] = signalTime > 50 ? 1 : 0;
  }
  // compute the temperature and humidity values 
  for ( bitNr = 15; bitNr >= 0; bitNr--) {
    humidity += data[bitNr] * Math.pow( 2, 15 - bitNr);
    temperature += data[bitNr + 16] * Math.pow( 2, 15 - bitNr);
  }
 
  // verify if were transmission-receptions errors
  // which means the checksum (last 8 transmission bits) 
  // should be equal with the last 8 bits of the sum of 
  // the first four transmitted bytes 
  bitGroup[0] = humidity >> 8;
  bitGroup[1] = humidity & 0xFF;
  bitGroup[2] = temperature >> 8;
  bitGroup[3] = temperature & 0xFF;
  // transform the checksum (last 8 bits of the transmission) to decimal
  for ( bitNr = 39; bitNr >= 32; bitNr--) {
    checksum += data[bitNr] * Math.pow( 2, 39 - bitNr);
  }
  // erroneous data, need to get the data again from the sensor!
  if ( checksum !== ((bitGroup[0] + bitGroup[1] + bitGroup[2] + bitGroup[3]) & 0xFF)) {
    return readSensor( callback);
  }
  // get the decimal values of the temperature and humidity
  // NOTE: need to divide by 10 the 16 bits value to get the real decimal value
  humidity /= 10.0;
  temperature /= 10.0;  
  // data read correctly, now invoke the callback
  if ( typeof callback === 'function') {
    callback( temperature, humidity);
  }
};
    
// DEBUG: display data read from sensor
readSensor( function ( temperature, humidity) {
  console.log( "Temperature: " + temperature + "°C, Humidity: " + humidity + "%");
});