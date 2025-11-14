#include <iostream>

#include "gpio/gpio.h"

int main() {
    Gpio::Info gpioInfo;

    Gpio::init(gpioInfo);

    return 0;
}
