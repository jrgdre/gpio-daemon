#ifndef GPIO_H
#define GPIO_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

#include <QTime>
#include <QThread>
#include "worker.h"

#define SERVICE_NAME "com.kimmoli.gpio"


/* main class */

class Gpio: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", SERVICE_NAME)

public:
    Gpio();

    ~Gpio()
    {
        setInterruptEnable(false, QString());
    }

public slots:

    /* Returns current state of gpio pin */
    int getGpioState();

    /* Sets current gpio state */
    void setGpioState(int state);

    /* Simplifiers, to use without payload */
    void setGpioStateLow()
    {
            setGpioState( 0 );
    }
    void setGpioStateHigh()
    {
            setGpioState( 1 );
    }

    /* Enables gpioStateChanged signal from edge */
    void gpioInterruptEnable(QString edge);

    /* Disabpes gpioStateChanged signal */
    void gpioInterruptDisable();

    /* Sets debounce timeout in milliseconds */
    void setGpioInterruptDebounceTimeout(int timeout);


signals:
    /* "Interrupt" */
    void gpioStateChanged(int state);


private slots:

    /* interrupts */
    int setInterruptEnable(bool turn, QString edge);
    void handleGpioInterrupt();
    void handleDebounceTimer();

private:
    QThread *thread;
    Worker *worker;

    QTimer *debounce;
    int debounceTimeout;

    QMutex mutex;

    bool interruptsEnabled;

    int gpio_fd;

    QString edges;
};



#endif // GPIO_H
