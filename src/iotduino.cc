#include <node.h>
#include <v8.h>
#include "../../common/src/Duino.h"

using namespace v8;

#define DIGITAL_PIN 2
#define ANALOG_PIN 5;

void testDigitalRead ( long n) {
  long i = 0;
  printf("TESTING: digitalRead on pin 2\n\r");
  pinMode(DIGITAL_PIN, INPUT);
  for ( i = 0; i < n; i++) {
    digitalRead( DIGITAL_PIN);
  }
}

void testDigitalWrite ( long n) {
  long i = 0;
  printf("TESTING: digitalWrite on pin 2\n\r");
  pinMode(DIGITAL_PIN, OUTPUT);
  for ( i = 0; i < n; i++) {
    // alternate between HIGH and LOW
    digitalWrite( DIGITAL_PIN, i % 2);
  }
}

void showStatistics ( struct timeval startTime, struct timeval endTime, long operationsNo) {
  unsigned long diffTime = 1000 * (endTime.tv_sec - startTime.tv_sec) 
                           + (endTime.tv_usec - startTime.tv_usec) / 1000;
  printf("   %lu operations took %lu milliseconds ==> %lu operations per second, %.2f microseconds per operation \n\r",
         operationsNo, diffTime, operationsNo / diffTime * 1000, ((double)diffTime / operationsNo) * 1000.0);
} 

Handle<Value> methodPinMode( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 2) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'pinMode' method requires two parameters: ( unsigned integer ) pinNumber and ( unsigned integer) pinMode!"))
    );
  }
  int8_t pinNr = args[0]->ToInteger()->Value();
  int8_t pinM = args[1]->ToInteger()->Value();
  pinMode( pinNr, pinM);
  return scope.Close( Boolean::New( "true"));
}

Handle<Value> methodDigitalRead( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 1) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'digitalRead' method requires one parameter: ( unsigned integer ) pinNumber!"))
    );
  }
  int8_t pinNumber = args[0]->ToInteger()->Value();
  int8_t result = digitalRead( pinNumber);
  return scope.Close( Integer::New( result));
}

Handle<Value> methodDigitalWrite( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 2) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'digitalWrite' method requires two parameters: ( unsigned integer ) pinNumber and ( unsigned integer) pinState!"))
    );
  }
  int8_t pinNumber = args[0]->ToInteger()->Value();
  int8_t pinState = args[1]->ToInteger()->Value();
  digitalWrite( pinNumber, pinState);
  return scope.Close( Integer::New( pinState));
}

Handle<Value> methodPulseIn( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 3) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'pulseIn' method requires three parameters: ( unsigned integer ) pinNumber, ( unsigned integer) pinState and (unsigned long) timeout!"))
    );
  }
  int8_t pinNumber = args[0]->ToInteger()->Value();
  int8_t pinState = args[1]->ToInteger()->Value();
  int32_t timeout = args[2]->ToInteger()->Value();
  int32_t result = pulseIn( pinNumber, pinState, timeout);
  return scope.Close( Integer::New( result));
}

Handle<Value> methodDelay( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 1) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'delay' method requires one parameter: ( unsigned long ) milliseconds!"))
    );
  }
  int32_t milliseconds = args[0]->ToInteger()->Value();
  delay( milliseconds);
  return scope.Close( Integer::New( milliseconds));
}

Handle<Value> methodDelayMicroseconds( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 1) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'delayMicroseconds' method requires one parameter: ( unsigned long ) milliseconds!"))
    );
  }
  int32_t microseconds = args[0]->ToInteger()->Value();
  delayMicroseconds( microseconds);
  return scope.Close( Integer::New( microseconds));
}

void init(Handle<Object> target) {
  // call Duino resource initialization
  initDuino();
 /* long i = 0, n = 1000000, rep = 3;
  struct timeval startTime, endTime;
  gettimeofday(&startTime, NULL);
  testDigitalRead( n);
  gettimeofday(&endTime, NULL);
  showStatistics( startTime, endTime, n);  
  gettimeofday(&startTime, NULL);
  testDigitalWrite( n);
  gettimeofday(&endTime, NULL);
  showStatistics( startTime, endTime, n);*/
  target->Set( String::NewSymbol( "pinMode"),
      FunctionTemplate::New( methodPinMode)->GetFunction());
  target->Set( String::NewSymbol( "digitalWrite"),
      FunctionTemplate::New( methodDigitalWrite)->GetFunction());
  target->Set( String::NewSymbol( "digitalRead"),
      FunctionTemplate::New( methodDigitalRead)->GetFunction());
  target->Set( String::NewSymbol( "pulseIn"),
      FunctionTemplate::New( methodPulseIn)->GetFunction());
  target->Set( String::NewSymbol( "delay"),
      FunctionTemplate::New( methodDelay)->GetFunction());
  target->Set( String::NewSymbol( "delayMicroseconds"),
      FunctionTemplate::New( methodDelayMicroseconds)->GetFunction());
}
NODE_MODULE(iotduino, init)
