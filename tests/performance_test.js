var duino = require( 'iotduino'), i = 0, n = 1000000, 
    startTime = null, endTime = null, time = 0;
 
duino.pinMode( 13, 1) 
startTime = new Date();
for ( i = 0; i < n; i++) {
  duino.digitalWrite( 13, 0);
}
endTime = new Date();
time = endTime - startTime;
console.log( "Digital write: " + n + " operations took " + time + " milliseconds ==> " + ( ( n / time) * 1000)
             + " operations per second, " + ( (time / n) * 1000.0) + " microseconds per operation.");

startTime = new Date();
for ( i = 0; i < n; i++) {
  duino.digitalRead( 13);
}
endTime = new Date();
time = endTime - startTime;
console.log( "Digital read: " + n + " operations took " + time + " milliseconds ==> " + ( ( n / time) * 1000)
             + " operations per second, " + ( (time / n) * 1000.0) + " microseconds per operation.");