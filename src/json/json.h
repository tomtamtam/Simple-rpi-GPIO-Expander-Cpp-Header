#pragma once

#include <iostream>

#include "../gpio/gpio.h"

namespace Json
{
  std::string parse(std::string filename);
  std::pair<int, std::vector<Gpio::Board>> parseToBoards(std::string filename);
};


