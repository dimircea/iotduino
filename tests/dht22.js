/* WORK IN PROGRESS: not ready yet! */


var duino = require( '../build/Release/iotduino'),
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
    signalTime = duino.micros() - startTime;
    if ( signalTime > 50) {
      console.log( "Step 1 error: expected pinState.HIGH for less than 40 microseconds !");
      return;
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.LOW) {
    signalTime = duino.micros() - startTime;
    if ( signalTime > 90) {
      console.log( "Step 2 error: expected pinState.LOW for less than 80 microseconds !");
      return
    }
  }
  startTime = duino.micros();
  while ( duino.digitalRead( dataPin) == pinState.HIGH) {
    signalTime = duino.micros() - startTime;
    if ( signalTime > 90) {
      console.log( "Step 3 error: expected pinState.LOW for less than 80 microseconds !");
      return;
    }
  }
  // read the 40 bits of data sent by the sensor
  while ( bitNr < totalBitsNr) {
    // receive the about 50 microseconds LOW signal, 
    // used as separator before two consecutive data bits
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.LOW) {
      signalTime = duino.micros() - startTime;
      if ( signalTime > 60) {
        //console.log( "Step 4 error: expected pinState.LOW for about 50 microseconds !");
        return;
      }
    }
    // receive the data bit
    startTime = duino.micros();
    while ( duino.digitalRead( dataPin) == pinState.HIGH) {
      signalTime = duino.micros() - startTime;
      if ( signalTime > 90) {
       // console.log( "Step 5 error: expected pinState.HIGH for less than 80 microseconds !");
        return;
      }
    }
    signalTime = duino.micros() - startTime;
    // signal is between 20-40 microseconds, then it is a '0' bit
    if (signalTime < 50) { 
      data[bitNr] = 0;
      //data[bitNr] = signalTime;
    } 
    // signal is between 60-80 microseconds, then it is a '1' bit
    else if ( signalTime < 100) {
      data[bitNr] = 1;
      //data[bitNr] = signalTime;
    } 
    // error reading the data bit, start again the process;
    else {
      console.log( "Error reading data bit! Starting over!");
      return;
    }
    bitNr++;
  }
  
  for ( bitNr = 0; bitNr < totalBitsNr; bitNr++) {
    if( bitNr % 8 === 0) {
      process.stdout.write('\n');
    } else {
      process.stdout.write(data[bitNr] + ", ");
    }
  }
  console.log( "Temperature: " + temperature + "°C");
  console.log( "Start collecting data...");
  
  // alternate the pin state between HIGH and LOW 
}, 2000);
