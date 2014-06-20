#ifndef GPIO_DAEMON_H
#define GPIO_DAEMON_H

#include "gpio.h"

void daemonize();
void signalHandler(int sig);

Gpio *gpio;

#endif /* GPIO_DAEMON_H */
