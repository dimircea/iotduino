#include <node.h>
#include <v8.h>
#include "Duino.h"

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

Handle<Value> methodAnalogRead( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 1) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'analogRead' method requires one parameter: ( unsigned integer ) pinNumber!"))
    );
  }
  int8_t pinNumber = args[0]->ToInteger()->Value();
  double result = analogRead( pinNumber);
  return scope.Close( Number::New( result));
}

Handle<Value> methodAnalogWrite( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 2) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'analogWrite' method requires two parameters: ( unsigned integer ) pinNumber and ( unsigned integer) value!"))
    );
  }
  uint8_t pinNumber = args[0]->ToInteger()->Value();
  int32_t value = args[1]->ToInteger()->Value();
  
  if ( value < 0 || value > MAX_PWM_LEVEL) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'analogWrite' method requires that the 'value' parameter is an integer in the range [0, 255]!"))
    );
  }
  analogWrite( pinNumber, (uint8_t) value);
  return scope.Close( Boolean::New( true));
}

Handle<Value> methodSetPwmFrequency( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() != 2) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'setPwmFrequency' method requires two parameters: ( unsigned integer ) pinNumber and ( unsigned integer) frequency!"))
    );
  }
  uint8_t pinNumber = args[0]->ToInteger()->Value();
  int32_t frequency = args[1]->ToInteger()->Value();
  if ( ( pinNumber != GPIO3) && ( pinNumber != GPIO5) && ( pinNumber != GPIO6) && ( pinNumber != GPIO9) && ( pinNumber != GPIO10) && ( pinNumber != GPIO11)) {
   return ThrowException(
      Exception::TypeError(
        String::New( "The 'setPwmFrequency' method requires that the 'pinNumber' parameter must be one of: GPIO3, GPIO5, GPIO6, GPIO9, GPIO10 or GPIO11!"))
    );
  } 
  if ( ( pinNumber == GPIO5) || ( pinNumber == GPIO6)) {
    if ( ( frequency != 195) && ( frequency != 260) && ( frequency != 390) && ( frequency != 520) && ( frequency != 781)) {
      return ThrowException(
        Exception::TypeError(
          String::New( "The 'setPwmFrequency' method requires that the 'frequency' parameter for GPIO5 or GPIO6 is one of the following values: 195, 260, 390, 520, 781!"))
      );
    }
  } else {
    if ( ( frequency < MIN_PWMTMR_FREQ) || ( frequency > MAX_PWMHW_FREQ)) {
      return ThrowException(
        Exception::TypeError(
          String::New( "The 'setPwmFrequency' method requires that the 'frequency' parameter for GPIO3, GPIO9, GPIO10 and GPIO11 must be in the range [126, 2000] Hz!"))
      );
    }
  }
  int result = setPwmFrequency( pinNumber, frequency);
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

Handle<Value> methodMicros( const Arguments& args) {
  HandleScope scope;
  if ( args.Length() > 0) {
    return ThrowException(
      Exception::TypeError(
        String::New( "The 'micros' method takes NO parameters!"))
    );
  }
  return scope.Close( Integer::New( micros()));
}

/**
 * Define the pin mode constants.
 * Access is made as PinMode.mode ( e.g. PinMode.OUTPUT)
 */
void definePinModes( Handle<Object> target) {
  HandleScope scope;
  Handle<Object> pinModesObj = Object::New();
  target->Set(String::NewSymbol("PinMode"), pinModesObj);
  NODE_DEFINE_CONSTANT(pinModesObj, INPUT);
  NODE_DEFINE_CONSTANT(pinModesObj, OUTPUT);
  NODE_DEFINE_CONSTANT(pinModesObj, INPUT_PULLUP);
}

/**
 * Define the pin state constants.
 * Access is made as PinState.state ( e.g. PinState.HIGH)
 */
void definePinStates( Handle<Object> target) {
  HandleScope scope;
  Handle<Object> pinStatesObj = Object::New();
  target->Set(String::NewSymbol("PinState"), pinStatesObj);
  NODE_DEFINE_CONSTANT(pinStatesObj, HIGH);
  NODE_DEFINE_CONSTANT(pinStatesObj, LOW);
}

/**
 * Define the pins constants.
 * Access is made as Pin.GPIOxx ( e.g. Pin.GPIO13)
 */
void definePins( Handle<Object> target) {
  HandleScope scope;
  Handle<Object> pinsObj = Object::New();
  target->Set(String::NewSymbol("Pins"), pinsObj);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO0);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO1);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO2);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO3);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO4);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO5);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO6);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO7);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO8);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO9);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO10);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO11);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO12);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO13);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO14);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO15);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO16);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO17);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO18);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO19);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO20);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO21);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO22);
  NODE_DEFINE_CONSTANT(pinsObj, GPIO23);
  
  NODE_DEFINE_CONSTANT(pinsObj, A0);
  NODE_DEFINE_CONSTANT(pinsObj, A1);
  NODE_DEFINE_CONSTANT(pinsObj, A2);
  NODE_DEFINE_CONSTANT(pinsObj, A3);
  NODE_DEFINE_CONSTANT(pinsObj, A4);
  NODE_DEFINE_CONSTANT(pinsObj, A5);
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
  // PinMode constants
  definePinModes( target);
  // PinState constants
  definePinStates( target);
  // Pin constants
  definePins( target);
  // define the accessible methods
  target->Set( String::NewSymbol( "pinMode"),
      FunctionTemplate::New( methodPinMode)->GetFunction());
  target->Set( String::NewSymbol( "digitalWrite"),
      FunctionTemplate::New( methodDigitalWrite)->GetFunction());
  target->Set( String::NewSymbol( "digitalRead"),
      FunctionTemplate::New( methodDigitalRead)->GetFunction());
  target->Set( String::NewSymbol( "pulseIn"),
      FunctionTemplate::New( methodPulseIn)->GetFunction());
  target->Set( String::NewSymbol( "analogRead"),
      FunctionTemplate::New( methodAnalogRead)->GetFunction());
  target->Set( String::NewSymbol( "analogWrite"),
      FunctionTemplate::New( methodAnalogWrite)->GetFunction());
  target->Set( String::NewSymbol( "setPwmFrequency"),
      FunctionTemplate::New( methodSetPwmFrequency)->GetFunction());
  target->Set( String::NewSymbol( "delay"),
      FunctionTemplate::New( methodDelay)->GetFunction());
  target->Set( String::NewSymbol( "delayMicroseconds"),
      FunctionTemplate::New( methodDelayMicroseconds)->GetFunction());
  target->Set( String::NewSymbol( "micros"),
      FunctionTemplate::New( methodMicros)->GetFunction());
}
NODE_MODULE(iotduino, init)
