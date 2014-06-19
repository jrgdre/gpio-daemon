#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include "gpio.h"
#include "toh.h"
#include "gpio-dbus.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtDBus/QtDBus>
#include <QDBusConnection>
#include <QDBusMessage>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    daemonize();

    setlinebuf(stdout);
    setlinebuf(stderr);

    printf("Starting gpio daemon. Version %s build %s %s\n", GITHASH, __DATE__, __TIME__);

//    if (!QDBusConnection::systemBus().isConnected())
//    {
//        printf("Cannot connect to the D-Bus systemBus\n%s\n", qPrintable(QDBusConnection::systemBus().lastError().message()));
//        exit(EXIT_FAILURE);
//    }
//    printf("Connected to D-Bus systembus\n");

    printf("Environment %s\n", qPrintable(getenv ("DBUS_SESSION_BUS_ADDRESS")));

    if (!QDBusConnection::sessionBus().isConnected())
    {
        printf("Cannot connect to the D-Bus sessionBus\n%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(EXIT_FAILURE);
    }
    printf("Connected to D-Bus sessionbus\n");

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME))
    {
        printf("Cannot register service to systemBus\n%s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(EXIT_FAILURE);
    }

    printf("Registered %s to D-Bus systembus\n", SERVICE_NAME);

    Gpio gpio;

    QDBusConnection::sessionBus().registerObject("/", &gpio, QDBusConnection::ExportAllSlots);


    return app.exec();

}


void daemonize()
{
	/* Change the file mode mask */
	umask(0);

	/* Change the current working directory */
	if ((chdir("/tmp")) < 0) 
		exit(EXIT_FAILURE);

	/* register signals to monitor / ignore */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signalHandler); /* catch hangup signal */
	signal(SIGTERM,signalHandler); /* catch kill signal */
}


void signalHandler(int sig) /* signal handler function */
{
	switch(sig)
	{
		case SIGHUP:
			/* rehash the server */
            printf("Received signal SIGHUP\n");
			break;		
		case SIGTERM:
			/* finalize the server */
            printf("Received signal SIGTERM\n");
            deinitOled();
            controlVdd(0);
			exit(0);
			break;		
	}	
}
