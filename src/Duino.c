#include "Duino.h"

int gpioPinSrc[MAX_GPIO_NUM + 1];
int gpioModeSrc[MAX_GPIO_NUM + 1];
int gpioADCSrc[MAX_ADC_NUM + 1];
static const char *pwm_dev = "/dev/pwmtimer";

typedef struct tagPWM_Config {
    int channel;
    int dutycycle;
} PWM_Config,*pPWM_Config;

typedef struct tagPWM_Freq {
    int channel;
    int step;
    int pre_scale;
    unsigned int freq;
} PWM_Freq,*pPWM_Freq;

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
  if ( mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP) {
    hardwarePinMode( pin, mode);  
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

uint8_t digitalRead(uint8_t pin) {
  char  buf[4];    
  uint8_t ret = -1;     
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

int analogRead( uint8_t pin) {
  char str[10];
  char buf[32];    
  int ret = -1;
  char *p = NULL;   
  if ( pin <= A5 ) {
    memset( buf, 0, sizeof( buf));
    lseek( gpioADCSrc[pin], 0, SEEK_SET);
    ret = read( gpioADCSrc[pin], buf, sizeof( buf));
    if ( ret <= 0 ) {
        fprintf( stderr, "Failed to read ADC %d !\n", pin);
        exit( -1);
    }
    memset( ( void *)str, 0, sizeof( str));
    sprintf( str, "adc%d", pin);
    p = strstr( buf, str) + strlen( str) + 1;
    sscanf( p, "%d", &ret);
  } else if ( pin > A5 && pin <= MAX_ADC_NUM ) {
    // TODO: implement SPI adc reading
    //ret = readSpiAdc( pin - 6);
  } else {
    fprintf( stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
    exit( -1);
  }      
  return ret;
}

void analogWrite( uint8_t pin, uint8_t value) {
  int ret = -1;
  int fd = -1;
  int val = 0;
  PWM_Config pwmconfig;
  pwmconfig.channel = pin;
  pwmconfig.dutycycle = value;
  // bound limits [0, 255] are assured because of the uint8_t type, no need to check them
  if ( pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11) {
    fd = open( pwm_dev, O_RDONLY);
    if ( fd < 0 ) {
      pabort( "analogWrite: opening PWM device failed!");
    }      
    switch ( pin) {
      case 5:
      case 6:
        ret = ioctl( fd, HWPWM_DUTY, &pwmconfig);
        if ( ret < 0) {
          pabort( "analogWrite: can't set HWPWM_DUTY");
        }  
        break;
      case 3:
      case 9:    
      case 10:   
      case 11:   
        ret = ioctl( fd, PWM_CONFIG, &pwmconfig);
        if ( ret < 0) {
          pabort( "analogWrite: can't set PWM_CONFIG");   
        }
        ret = ioctl(fd, PWMTMR_START, &val);
        if (ret < 0) {
          pabort("analogWrite: can't set PWMTMR_START");
        }
        break;

      default:   
        break;
    }
    if( fd) {
      close(fd);
    }
  } else {
    fprintf( stderr, "%s ERROR: invalid pin, pin=%d\n", __FUNCTION__, pin);
    exit(-1);
  }      
}

int setPwmFrequency( uint8_t pin, uint16_t freq) {
  int ret = -1;
  int fd = -1;
  PWM_Freq pwmfreq;
  if ( pin == GPIO3 || pin == GPIO5 || pin == GPIO6 || pin == GPIO9 || pin == GPIO10 || pin == GPIO11) {
    pwmfreq.channel = pin;
    pwmfreq.freq = freq;
    pwmfreq.step = 0;
    fd = open( pwm_dev, O_RDONLY);
    if ( fd < 0 ) {
      pabort( "setPwmFrequency: open pwm device fail!\n");
    }
    if ( pin == GPIO5 || pin == GPIO6) {  
      if ( ( freq == 195) || ( freq == 260) || ( freq == 390) || ( freq == 520) || ( freq == 781)) {
        ret = ioctl(fd, PWM_FREQ, &pwmfreq);
        if ( ret < 0) {
          pabort("setPwmFrequency: can't set PWM_FREQ!\n");
        }
      } else {
        fprintf(stderr, "setPwmFrequency: invalid frequency value - it must be one of [ 195, 260, 390, 520, 781], pin=%d!\n", pin);
      }
    } 
    // pins 3, 9, 10, 11
    else {
      if ((freq >= MIN_PWMTMR_FREQ) && (freq <= MAX_PWMTMR_FREQ)){
        ret = ioctl( fd, PWMTMR_STOP, &pwmfreq.channel);
        if ( ret < 0) {
          pabort("setPwmFrequency: can't set PWMTMR_STOP!\n");  
        }               
        ret = ioctl( fd, PWM_FREQ, &pwmfreq);
        if ( ret < 0) {
          pabort( "setPwmFrequency: can't set PWM_FREQ!\n");   
        }
      } else {
        fprintf( stderr, "setPwmFrequency: invalid frequency[%d,%d], pin=%d!\n", MIN_PWMTMR_FREQ, MAX_PWMTMR_FREQ, pin);
      }
    }
    if( fd) {
      close(fd);
    }
    return pwmfreq.step;
  } else {
    fprintf( stderr, "setPwmFrequency: invalid pin, [pin=%d]! Only pin 3, 5, 6, 9, 10 and 11 can be used for PWM!\n", pin);
    exit( -1);
  } 
  return 0;
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