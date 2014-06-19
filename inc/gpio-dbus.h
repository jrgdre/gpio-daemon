#ifndef GPIO_DBUS_H
#define GPIO_DBUS_H

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
        /* Disable everything */
        setInterruptEnable(false);
    }

public slots:
    QString testSomething();

private slots:

    /* interrupts */
    int setInterruptEnable(bool turn);
    void handleGpioInterrupt();

private:
    QThread *thread;
    Worker *worker;

    QMutex mutex;

    static bool interruptsEnabled;

    int gpio_fd;
};



#endif // GPIO-DBUS_H
