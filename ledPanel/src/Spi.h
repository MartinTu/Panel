/*
 * Spi.h
 *
 *  Created on: 31.01.2013
 *      Author: martin
 */

#ifndef SPI_H_
#define SPI_H_

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/time.h>//
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Utils.h"
#include "MyException.h"

class SPI
{
    public:
        SPI();
        virtual ~SPI();

        int init();
        int sendData(const uint8_t *data, uint32_t dataLength);

    protected:
        static int spi_transfer(int fd, const uint8_t* tx, uint32_t dataLength);

        int fd;
        static const char *device;
        static uint8_t mode;
        static uint8_t bits;
        static uint8_t bitorder;
        static uint32_t speed;
        static uint16_t delay;
};

#endif /* SPI_H_ */
