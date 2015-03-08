/*
 * Spi.cpp
 *
 *  Created on: 31.01.2013
 *      Author: martin
 */

#include "Spi.h"

using namespace std;

#ifdef LAPTOP
const char *SPI::device = "/dev/null";  // FIXME Debug purpose
#else
const char *SPI::device = "/dev/spidev0.0";

uint8_t SPI::mode = 0;
uint8_t SPI::bits = 8;
uint8_t SPI::bitorder = 0;
uint32_t SPI::speed = 1e6;
uint16_t SPI::delay;

/* wiringPi:
 * changeOwner:
 *      Change the ownership of the file to the real userId of the calling
 *      program so we can access it.
 *********************************************************************************
 */
static void changeOwner(string file)
{
    uid_t uid = getuid();
    uid_t gid = getgid();

    if (chown(file.c_str(), uid, gid) != 0)
    {
        if (errno == ENOENT)        // Warn that it's not there
        fprintf(stderr,
                "Display SPI-Loader: Warning: File not present: %s\n",
                file.c_str());
        else
        {
            fprintf(stderr,
                    "Display SPI-Loader: Unable to change ownership of %s: %s\n",
                    file.c_str(), strerror(errno));
            exit(1);
        }
    }
}

/* wiringPi:
 * moduleLoaded:
 *      Return true/false if the supplied module is loaded
 *********************************************************************************
 */

static int moduleLoaded(string modName)
{
    int len = modName.size();
    int found = 0;
    FILE *fd = fopen("/proc/modules", "r");
    char line[80];

    if (fd == NULL)
    {
        fprintf(stderr, "gpio: Unable to check modules: %s\n", strerror(errno));
        exit(1);
    }

    while (fgets(line, 80, fd) != NULL)
    {
        if (strncmp(line, modName.c_str(), len) != 0)
        continue;

        found = 1;
        break;
    }

    fclose(fd);

    return found;
}
#endif

/*
 * also from wiringPi
 *********************************************/
static int loadSPI()
{
#ifndef LAPTOP
    string module1 = "spidev";
    string module2 = "spi_bcm2708";
    string file1 = "/dev/spidev0.0";
    string file2 = "/dev/spidev0.1";

    char cmd[80];
    char spiBuf[32];

    if (!moduleLoaded(module1))
    {
        sprintf(cmd, "modprobe %s%s", module1.c_str(), spiBuf);
        system(cmd);
    }

    if (!moduleLoaded(module2))
    {
        sprintf(cmd, "modprobe %s", module2.c_str());
        system(cmd);
    }

    if (!moduleLoaded(module2))
    {
        cerr << "Display SPI-Loader: Unable to load %s\n" << endl;
        return -1;
    }

    sleep(1);   // To let things get settled

    changeOwner(file1);
    changeOwner(file2);
#endif
    return 0;
}

SPI::SPI()
{
    int rt;
    struct sched_param param;

#ifdef LAPTOP
    if (!Utils::fileExists("/dev", "null"))  // FIXME debug purpose
#else
    if (!Utils::fileExists("/dev", "spidev0.0"))
#endif
    {
        cout << "no spi device found, try to load it" << endl;
        if (loadSPI())
        {
            cerr << "unable to load spi." << endl;
            throw MyException("unable to load spi");
        }
    }

    this->fd = open(device, O_RDWR);
    if (fd < 0)
    {
        cerr << "can't open device: " << strerror(errno) << endl;
        throw MyException("can't open device: " + string(strerror(errno)));
    }

    param.sched_priority = 51; 	//sched_get_priority_max(SCHED_FIFO);
    rt = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    if (0 != rt)
    {
        throw MyException("SPI: unable to change schedule parameters: " + string(strerror(rt)));
    }
    Utils::printStars();
    cout << "* SPI thread:" << endl;
    Utils::display_thread_sched_attr();
}

SPI::~SPI()
{
    close(fd);
}

/**
 * Initialize the SPI interface
 */
int SPI::init()
{
#ifndef LAPTOP
    int ret = 0;
    // spi mode
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
    {
        cerr << "can't set spi mode: " << strerror(errno) << endl;
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
    {
        cerr << "can't get spi mode: " << strerror(errno) << endl;
        return -1;
    }

    //bits per word
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        cerr << "can't set bits per word: " << strerror(errno) << endl;
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        cerr << "can't get bits per word: " << strerror(errno) << endl;
        return -1;
    }

    // max speed Hz
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        cerr << "can't set max speed hz: " << strerror(errno) << endl;
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        cerr << "can't get max speed hz: " << strerror(errno) << endl;
        return -1;
    }

    // bit order
    //Zero indicates MSB-first;
    ret = ioctl(fd, SPI_IOC_RD_LSB_FIRST, &bitorder);
    if (ret == -1)
    {
        cerr << "can't set bit order: " << strerror(errno) << endl;
        return -1;
    }

    ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &bitorder);
    if (ret == -1)
    {
        cerr << "can't get bit order: " << strerror(errno) << endl;
        return -1;
    }

//    printf("spi mode: %d\n", mode);
//    printf("bits per word: %d\n", bits);
//    printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);
//    printf("bit order: %d\n", bitorder);
#endif
    return fd;
}

/**
 * send data to the LDP6803 chip
 */
int SPI::sendData(const uint8_t *data, uint32_t dataLength)
{
//    unsigned long time1 = Utile::getTime();
    int rt = spi_transfer(this->fd, data, dataLength + 4);
    if (rt < 0)
        cout << "ERROR during spi transfer: " << strerror(errno) << endl;
//    cout << (Utile::getTime() - time1) / 1000.0 << "ms" << endl;

    return 0;
}

/**
 * send data over the SPI bus
 */
int SPI::spi_transfer(int fd, const uint8_t* tx, uint32_t dataLength)
{
    int ret = 0;
#ifndef LAPTOP
    uint8_t* rx = new uint8_t[dataLength];

    struct spi_ioc_transfer tr;
    memset(&tr,0,sizeof(tr));
    tr.tx_buf = (unsigned long) tx;
    tr.rx_buf = (unsigned long) rx;
    tr.len = dataLength;
    tr.delay_usecs = delay;
    tr.speed_hz = speed;
    tr.bits_per_word = bits;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

    delete[] rx;
#endif
    return ret;
}

