#include "Duino.h"

int gpioPinSrc[MAX_GPIO_NUM + 1];
int gpioModeSrc[MAX_GPIO_NUM + 1];
int gpioADCSrc[MAX_ADC_NUM + 1];

int writeToFile( int fd, char *str, int len) {
  int ret = -1;
  lseek(fd, 0, SEEK_SET);
  ret = write(fd, str, len);
  if ( ret <= 0 ) {
    fprintf(stderr, "write %d failed\n", fd);
    return -1;
  }
  return ret;
}

void pabort(const char *s) {
  perror(s);
  abort();
}

/**
 * Perform the initialization of the pin access resources.
 * NOTE: the sources are defined in PinSource.h
 */
void initDuino() {
  int i;
  char path[1024];
  for( i = 0; i <= MAX_GPIO_NUM; ++i) {
    memset( path, 0, sizeof( path));
    sprintf( path, "%s%s%d", GPIO_PIN_DIR, GPIO_IF_PREFIX, i);
    gpioPinSrc[i] = open( path, O_RDWR);
    if ( gpioPinSrc[i] < 0 ) {
      fprintf( stderr, "Open pin resource %s failed!\n", path);
      return;
    }
    memset( path, 0, sizeof( path));
    sprintf( path, "%s%s%d", GPIO_MODE_DIR, GPIO_IF_PREFIX, i);
    gpioModeSrc[i] = open(path, O_RDWR);
    if ( gpioModeSrc[i] < 0 ) {
      fprintf( stderr, "Open pin mode resource %s failed!\n", path);
      return;
    } 
  } 
  for( i = 0; i <= 5; ++i) {
    memset( path, 0, sizeof(path));
    sprintf( path, "%s%d", ADC_IF, i);
    gpioADCSrc[i] = open(path, O_RDONLY);
    if ( gpioADCSrc[i] < 0 ) {
      fprintf( stderr, "Open ADC pin resource %s failed!\n", path);
      return;
    }
  }
}

void hardwarePinMode( uint8_t pin, uint8_t mode) {
  int ret = -1;
  char buf[4];
  if ( pin <=  MAX_GPIO_NUM && mode <= MAX_GPIO_MODE_NUM) {
    memset( (void *)buf, 0, sizeof( buf));
    sprintf( buf, "%d", mode);
    ret = writeToFile( gpioModeSrc[pin], buf, sizeof( buf));
    if ( ret <= 0 ) {
      fprintf (stderr, "Write GPIO %d mode failed!\n", pin);
      exit( -1);
    }
  } else {
    fprintf( stderr, "%s ERROR: invalid pin or mode, pin=%d, mode=%d\n",
    __FUNCTION__, pin, mode);
    exit( -1);
  }
}

void pinMode( uint8_t pin, uint8_t mode) {
  int ret = -1, fd = -1;
  unsigned long val = pin;
  switch ( pin) {         
    case 3:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:      
      fd = open( "/dev/pwmtimer", O_RDONLY);
      if ( fd < 0 ) {
        pabort( "Open pwm device failed!\n");
      }          
      ret = ioctl( fd, 0x102, &val); //PWMTMR_STOP
      if ( ret < 0) {
        pabort("Can't set PWMTMR_STOP !");
      }
      if( fd) {
        close( fd);
      }       
      break;
    default:
      break; 
  }
  switch ( mode) {         
    case INPUT:
      break;
    case OUTPUT:  
      hardwarePinMode( pin, mode);  
      break;  
    case INPUT_PULLUP:
      hardwarePinMode( pin, mode);  
      break;
    default:
      break; 
  }
}

void digitalWrite(uint8_t pin, uint8_t value) {
  char buf[4]; 
  int ret = -1;
  if ( pin <=  MAX_GPIO_NUM && ( value == HIGH || value == LOW)) {
    memset( (void *)buf, 0, sizeof(buf));
    sprintf( buf, "%d", value);
    ret = writeToFile( gpioPinSrc[pin], buf, sizeof( buf));
    if ( ret <= 0 ) {
      fprintf( stderr, "Write GPIO %d  failed!\n", pin);
      exit(-1);
    }
  } else {
    fprintf(stderr, "%s ERROR: invalid pin or mode, pin=%d, value=%d\n",
      __FUNCTION__, pin, value);
    exit(-1);
  }    
}

int digitalRead(uint8_t pin) {
  char  buf[4];    
  int ret = -1;     
  if ( pin <= MAX_GPIO_NUM) {
    memset( (void *)buf, 0, sizeof( buf));
    lseek( gpioPinSrc[pin], 0, SEEK_SET);
    ret = read( gpioPinSrc[pin], buf, sizeof(buf));
    if ( ret <= 0) {
      fprintf(stderr, "read gpio %d failed\n", pin);
      exit(-1);
    }
    ret = buf[0] - '0';
    switch( ret){
      case LOW:
        break;
      case HIGH:
        break;
      default:
        ret = -1;
        break;
    }
  } else {
    fprintf( stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
    exit(-1);
  }      
  return ret;
}

unsigned long pulseIn( uint8_t pin, uint8_t state, unsigned long timeout) {
  unsigned long start, value;
  pinMode(pin, INPUT);
  start = micros();
  while( digitalRead( pin) == state ) {
    if( micros() - start > timeout ) {
      return 0;
    }  
  }
  while( digitalRead( pin) != state ) {
    if( micros() - start > timeout ) {
      return 0;
    }
  }
  value = micros();
  while( digitalRead(pin) == state) {
    if( micros() - start > timeout) {
      return 0;
    }
  }   
  return micros() - value;
}

unsigned long millis() {
  struct timeval tv; 
  gettimeofday (&tv , NULL); 
  return ( tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

unsigned long micros() {
  struct timeval tv;
  gettimeofday (&tv , NULL);
  return ( tv.tv_sec * 1000000 + tv.tv_usec);
}

void delay( unsigned long ms) {
  usleep( ms * 1000);
}

void delayMicroseconds( unsigned long us) {
  usleep( us);
}