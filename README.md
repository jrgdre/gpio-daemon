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


Example
-------

On Jolla console type:

[nemo@Sailfish ~]$ dbus-send --session --print-reply --dest=com.kimmoli.gpio / com.kimmoli.gpio.setGpoiInterruptDebounceTimeout int32:100

[nemo@Sailfish ~]$ dbus-send --session --print-reply --dest=com.kimmoli.gpio / com.kimmoli.gpio.gpioIntteruptEnable string:"falling"

[nemo@Sailfish ~]$ dbus-monitor "type='signal',sender='com.kimmoli.gpio',path='/',interface='com.kimmoli.gpio',member='gpioStateChanged'"

Now, whenever we pull down the TOH - INT line we see something similar to:

signal time=1486871222.323090 sender=:1.152 -> destination=(null destination) serial=16 path=/; interface=com.kimmoli.gpio; member=gpioStateChanged

   int32 0
