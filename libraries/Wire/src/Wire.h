/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
  Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
  Modified November 2017 by Chuck Todd <stickbreaker on GitHub> to use ISR and increase stability.
*/

#ifndef TwoWire_h
#define TwoWire_h

#include <esp32-hal.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Stream.h"

#define I2C_BUFFER_LENGTH 128
typedef void(*user_onRequest)(void);
typedef void(*user_onReceive)(uint8_t*, int);

class TwoWire: public Stream
{
protected:
    uint8_t num;
    int8_t sda;
    int8_t scl;
    i2c_t * i2c;

    uint8_t rxBuffer[I2C_BUFFER_LENGTH];
    uint16_t rxIndex;
    uint16_t rxLength;
    uint16_t rxQueued; //@stickBreaker

    uint8_t txBuffer[I2C_BUFFER_LENGTH];
    uint16_t txIndex;
    uint16_t txLength;
    uint16_t txAddress;
    uint16_t txQueued; //@stickbreaker

    uint8_t transmitting;
/* slave Mode, not yet Stickbreaker
		static user_onRequest uReq[2];
		static user_onReceive uRcv[2];
    void onRequestService(void);
    void onReceiveService(uint8_t*, int);
*/
    i2c_err_t last_error; // @stickBreaker from esp32-hal-i2c.h
    i2c_err_t processQueue(uint32_t *readCount);
    uint16_t _timeOutMillis;
    bool _dump;

public:
    TwoWire(uint8_t bus_num);
    void begin(int sda=-1, int scl=-1, uint32_t frequency=100000);
    void setClock(uint32_t);
    void beginTransmission(uint16_t);
    uint8_t endTransmission(bool);
		uint8_t	requestFrom(uint16_t address, uint8_t size, bool sendStop);
//@stickBreaker for big blocks and ISR model
    i2c_err_t writeTransmission(uint16_t address, uint8_t* buff, uint16_t size, bool sendStop=true);
    i2c_err_t readTransmission(uint16_t address, uint8_t* buff, uint16_t size, bool sendStop=true);
		uint16_t 	requestFrom(uint16_t address, uint8_t* buf, uint16_t size, bool sendStop);
		uint8_t	transact(uint8_t readLen);
    uint16_t transact(uint8_t* readBuff, uint16_t readLen);
		uint8_t	lastError();
    char * getErrorText(uint8_t err);
    void dumpOn(){_dump=true;}
    void dumpOff(){_dump=false;}
    bool getDump(){return _dump;}
    void dumpInts();
    void dumpI2C(){i2cDumpI2c(i2c);}
    size_t getClock();
    void setTimeOut(uint16_t timeOutMillis);
    uint16_t getTimeOut();
//		
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint16_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
		uint8_t requestFrom(uint16_t, uint8_t);
    
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );


    size_t write(uint8_t);
    size_t write(const uint8_t *, size_t);
    int available(void);
    int read(void);
    int peek(void);
    void flush(void);

    void reset(void);

    inline size_t write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t)n);
    }
};

extern TwoWire Wire;

#endif
