/*
 * (C) 2014 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 *
 * toholed daemon, d-bus server call method functions.
 *
 *
 *
 *
 */


#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtDBus/QtDBus>
#include <QDBusArgument>
#include <QtCore/QTimer>
#include <QTime>
#include <QTimer>
#include <QThread>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <fcntl.h>

#include <poll.h>

#include "gpio.h"
#include "toh.h"


/* Main */
Gpio::Gpio()
{
    thread = new QThread();
    worker = new Worker();

    worker->moveToThread(thread);
    connect(worker, SIGNAL(gpioInterruptCaptured()), this, SLOT(handleGpioInterrupt()));
    connect(worker, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);

    interruptsEnabled = false;
    debounceTimeout = 100; /* Default debounce timeout 100 ms */
    edges = "none";

    exportGpio();

    gpio_fd = -1;

    debounce = new QTimer(this);
    debounce->setInterval( debounceTimeout );
    debounce->setSingleShot(true);

    connect(debounce, SIGNAL(timeout()), this, SLOT(handleDebounceTimer()));
}


/* DBus Exposed call methods */

int Gpio::getGpioState()
{
    char buf[20];
    int fd;

    fd = open("/sys/class/gpio/gpio" GPIO_INT "/value", O_RDONLY | O_NONBLOCK);
    read(fd, buf, 1);
    close(fd);

    return buf[0] == 49;
}

void Gpio::setGpioState(int state)
{
    printf("setGpioState(%d)\n", state);
}


void Gpio::gpioInterruptEnable(QString edge)
{
    edges = edge;
    setInterruptEnable(true, edge);
}

void Gpio::gpioInterruptDisable()
{
    setInterruptEnable(false, QString());
}

void Gpio::setGpioInterruptDebounceTimeout(int timeout)
{
    debounceTimeout = timeout;
    debounce->setInterval(debounceTimeout);
}


/* Enable or disable interrupts */

int Gpio::setInterruptEnable(bool turn, QString edge)
{
    if (turn)
    {
        /* if enabling check that 'edge' is valid */
        if (!(!QString::localeAwareCompare( edge, "falling") ||
                !QString::localeAwareCompare( edge, "rising") ||
                !QString::localeAwareCompare( edge, "both") ||
                !QString::localeAwareCompare( edge, "none") ))
        {
            printf("Invalid edge '%s'' provided\n", qPrintable(edge));
            return -1;
        }
    }

    worker->abort();
    thread->wait(); // If the thread is not running, this will immediately return.

    /* Release interrupt first if it is already */
    if (gpio_fd >= 0)
    {
        releaseTohInterrupt(gpio_fd);
        gpio_fd = -1;
    }

    if (turn) /* Enable */
    {
        printf("Enabling interrupt\n");

        gpio_fd = getTohInterrupt(edge.toUtf8().data());

        if (gpio_fd < 0)
        {
            printf("Failed to register toh gpio interrupt\n");
        }
        else
        {
            worker->requestWork(gpio_fd);

            printf("Worker started\n");

            interruptsEnabled = true;
        }
    }
    else
    {
        interruptsEnabled = false;
        printf("Interrupt disabled succesfully\n");
    }

    return interruptsEnabled;
}


/* GPIO interrupt handler */

void Gpio::handleGpioInterrupt()
{
    if (!debounce->isActive())
        debounce->start();
}

void Gpio::handleDebounceTimer()
{
    int state = getGpioState();

    if (edges == "both" || (edges == "rising" && state == 1) || (edges == "falling" && state == 0))
    {
        printf("%s edge\n", state ? "rising" : "falling");
        emit gpioStateChanged(state);
    }
}
