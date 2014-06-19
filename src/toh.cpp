/*
 * (C) 2014 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 *
 * toholed daemon, TOH Low level control funtions
 *
 *
 *
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"


int releaseTohInterrupt(int fdGpio)
{

    int fd;

    close(fdGpio);

    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT, strlen(GPIO_INT));
        close(fd);
    }
    return fd;

}


int getTohInterrupt()
{

    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT, strlen(GPIO_INT));
        close(fd);
    }

    fd = open("/sys/class/gpio/gpio" GPIO_INT "/edge", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT_EDGE, strlen(GPIO_INT_EDGE));
        close(fd);
    }
    else
        return -1; /* error */


    fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_RDONLY | O_NONBLOCK);

    return fd;
}


