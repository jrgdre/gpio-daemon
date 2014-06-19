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
#include <QThread>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <poll.h>

#include "gpio-dbus.h"
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

    setInterruptEnable(true);
}


/* DBus Exposed call methods */

QString Gpio::testSomething()
{
    printf("testing received\n");
    return QString("Sorry, there is nothing to test...");
}



/* Enable or disable interrupts */

int Gpio::setInterruptEnable(bool turn)
{
    if(turn)
    {
        mutex.lock();

        printf("Enabling interrupt\n");

        gpio_fd = getTohInterrupt();

        if (gpio_fd > -1)
            printf("TOH Interrupt registered\n");

        if (gpio_fd > -1)
        {
            worker->abort();
            thread->wait(); // If the thread is not running, this will immediately return.

            worker->requestWork(gpio_fd);

            printf("Worker started\n");

            interruptsEnabled = true;
            mutex.unlock();

            return 1;
        }
        else
        {
            printf("Failed to register TOH interrupt\n");
            interruptsEnabled = false;
            mutex.unlock();
            return -1;
        }
    }
    else
    {

        printf("Disabling interrupt\n");

        interruptsEnabled = false;

        worker->abort();
        thread->wait();
        delete thread;
        delete worker;

        releaseTohInterrupt(gpio_fd);

        mutex.unlock();
        return 1;
    }

}


/* GPIO interrupt handler */

void Gpio::handleGpioInterrupt()
{
    mutex.lock();

    /* do it here */
    printf("Got falling edge on TOH gpio");

    mutex.unlock();

}
