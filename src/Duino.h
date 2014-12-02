#ifndef Duino_h
#define Duino_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h> 
#include <sys/time.h>
#include "PinMode.h"
#include "PinState.h"
#include "PinSource.h"
#include "Pins.h"

typedef uint8_t boolean;
/** type definitions for well known types **/
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

#ifdef __cplusplus
  extern "C"{
#endif

void pabort(const char *s);

/**
 * Write a given value to a given system file.
 * @param fd
 *          file descriptor
 * @param *str
 *          pointer to the char value to write to file
 * @param len
 *          the length of the pointer representing the 
 *          char value(s) to write to the file.
 */
int writeToFile( int fd, char *str, int len);

/**
 * Initialize: gather resources for pin control.
 */
void initDuino();

/**
 * Enable the pin mode at the hardware level.
 * This method should only be called internally, and the
 * custom code must call <code>pinMode</code> instead.
 */
void hardwarePinMode( uint8_t pin, uint8_t mode);

/**
 * Define the pin mode for the specified pin.
 * @param pin
 *          the pin affected by this command
 * @param mode
 *          the mode on which to open the pin 
 *          (@see PinMode.h for possible values)
 */
void pinMode( uint8_t pin, uint8_t mode);

/**
 * Write a value to the specified pin.
 * @param pin
 *          the pin affected by this command
 * @param state
 *          the state to write on the pin
 *          (@see PinState.h for possible values)
 */
void digitalWrite( uint8_t pin, uint8_t state);

/**
 * Read the state of the specified pin (HIGH or LOW)
 * @param pin
 *          the pin from which to read 
 * @return HIGH (1) or LOW (0) state of the pin.
 *          (@see PinState.h for possible values)
 */
uint8_t digitalRead( uint8_t pin);

/**
 * Perform a pulse read. It detects how long the pin stays in the specified
 * state after the call of this method.
 * NOTE: the method automatically sets the specified pin to INPUT mode!
 * @param pin
 *          the pin to be used for pulse read
 * @param state
 *          the state on which the pin is expected to be read
 * @param timeout
 *          a time-out value (in microseconds) set as maximum expected time in that state
 */
unsigned long pulseIn( uint8_t pin, uint8_t state, unsigned long timeout);

/**
 * Read the analogous value for the specified pin (HIGH or LOW)
 * @param pin
 *          the pin from which to read 
 * @return the analogous value
 */
int analogRead( uint8_t pin);

/**
 * Return the number of milliseconds passed from 1970-01-01 00:00:00 +0000 (UTC)
 * (known as Epoch time) until the moment of calling this method.
 * NOTE: since the number can be very large, the type range is overpassed 
 *       which results in taking the last modulo value which pass in the type range.
 * @param return the number of milliseconds 
 */
unsigned long micros();

/**
 * Return the number of microseconds passed from 1970-01-01 00:00:00 +0000 (UTC)
 * (known as Epoch time) until the moment of calling this method.
 * NOTE: since the number can be very large, the type range is overpassed 
 *       which results in taking the last modulo value which pass in the type range.
 * @param return the number of microseconds 
 */
unsigned long micros();

/**
 * Produce a delay (pause in code execution)of a specified time ( in milliseconds).
 * @param ms
 *          the number of milliseconds to wait.
 */
void delay( unsigned long ms);

/**
 * Produce a delay (pause in code execution)of a specified time ( in microseconds).
 * @param us
 *          the number of microseconds to wait.
 */
void delayMicroseconds( unsigned long us);

#ifdef __cplusplus
  } // extern "C"
#endif

#endif