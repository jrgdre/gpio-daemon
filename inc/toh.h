
int setGpio(int state);
int setGpioDir(bool output);
void exportGpio();
void unexportGpio();
int getTohInterrupt(char *edge);
int releaseTohInterrupt(int fdGpio);

#define GPIO_INT "67"
#define GPIO_INT_EDGE_NONE "none"
#define POLL_TIMEOUT 1000
