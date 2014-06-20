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
#include "toh.h"

int setGpioDir(bool output)
{
    int fd = open("/sys/class/gpio/gpio" GPIO_INT "/direction", O_WRONLY);

    if (fd >= 0)
    {
        if (write (fd, output ? "out" : "in", output ? 3 : 2) != output ? 3 : 2)
        {
            printf("error in setting GPIO direction\n");
            close (fd);
            return false;
        }
        else
        {
            printf("direction set to %s\n", output ? "output" : "input");
            close(fd);
        }
    }
    else
    {
        return false;
    }

    return true;
}

int setGpio(int state)
{
    int fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_WRONLY);

    if (fd >= 0)
    {
        if (write (fd, state ? "1" : "0", 1) != 1)
        {
            printf("error in setting GPIO state\n");
            close (fd);
            return false;
        }
        else
        {
            printf("state set to %d\n", state);
            close(fd);
        }
    }
    else
    {
        printf("error in setting GPIO state\n");
        return false;
    }

    return true;
}


int releaseTohInterrupt(int fdGpio)
{

    int fd;

    close(fdGpio);

    fd = open("/sys/class/gpio/gpio" GPIO_INT "/edge", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT_EDGE_NONE, strlen(GPIO_INT_EDGE_NONE));
        close(fd);
    }
    else
        return -1; /* error */


    return fd;

}

void exportGpio()
{
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT, strlen(GPIO_INT));
        close(fd);
    }
}

void unexportGpio()
{
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, GPIO_INT, strlen(GPIO_INT));
        close(fd);
    }

}


int getTohInterrupt(char *edge)
{


    int fd;

    fd = open("/sys/class/gpio/gpio" GPIO_INT "/edge", O_WRONLY);

    if (!(fd < 0))
    {
        write (fd, edge, strlen(edge));
        close(fd);
    }
    else
        return -1; /* error */


    fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_RDONLY | O_NONBLOCK);

    return fd;
}

