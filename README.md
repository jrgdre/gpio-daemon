gpio-daemon
=======

Jolla TOH GPIO Control daemon



signal sender=:1.183 -> dest=(null destination) serial=126 path=/; interface=com.kimmoli.gpio; member=gpioStateChanged

   int32 0
   
signal sender=:1.183 -> dest=(null destination) serial=127 path=/; interface=com.kimmoli.gpio; member=gpioStateChanged

   int32 1


dbus-send --session --dest=com.kimmoli.gpio / com.kimmoli.gpio.setGpioInterruptDebounceTimeout int32:100

dbus-send --session --dest=com.kimmoli.gpio / com.kimmoli.gpio.gpioInterruptEnable string:"both"

dbus-send --session --dest=com.kimmoli.gpio / com.kimmoli.gpio.gpioInterruptEnable string:"falling"

dbus-send --session --dest=com.kimmoli.gpio / com.kimmoli.gpio.gpioInterruptEnable string:"rising"

dbus-send --session --dest=com.kimmoli.gpio / com.kimmoli.gpio.gpioInterruptDisable



dbus-send --session --print-reply --dest=com.kimmoli.gpio / com.kimmoli.gpio.getGpioState

   int32 1


