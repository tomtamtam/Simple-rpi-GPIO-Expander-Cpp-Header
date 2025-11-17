#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23017.h>

#include "gpio.h"
#include "../json/json.h"

using namespace Gpio

void Gpio::init(const Info& info)
{
    wiringPiSetup();

    for (int i = 0; i < info.boardCount; i++)
    {
        const int start_numb = 100 + i * 20;
        mcp23017Setup(start_numb, info.boards[i].address);

        for (int n = 0; n < 2; n++)
        {
            const int a_b_offset = n * 8;

            for (int j = 0; j < 8; j++)
            {
                const bool type = (n == 0 ? info.boards[i].board_a[j].type
                                          : info.boards[i].board_b[j].type);

                pinMode(start_numb + a_b_offset + j, type);

                if (type == OUTPUT)
                {
                    const bool value = (n == 0 ? info.boards[i].board_a[j].value
                                               : info.boards[i].board_b[j].value);
                    digitalWrite(start_numb + a_b_offset + j, value);
                }
            }
        }
    }

    Gpio::setPin(info, PinPosition(0, true, 0), LOW);
    Gpio::setPin(info, PinPosition(0, true, 1), HIGH);

    std::cout << "pausing\n";
    delay(50000);

    Gpio::setPin(info, PinPosition(0, true, 1), LOW);
    Gpio::setPin(info, PinPosition(0, true, 0), HIGH);

    std::cout << "finished\n";
}

Gpio::Info::Info()
{
    std::pair<int, std::vector<Board>> infoPair = Json::parseToBoards(CONFIG_FILENAME);
    boardCount = infoPair.first;
    boards = infoPair.second;
}

int Gpio::getPinIdx(PinPosition position)
{
    return 100 + position.nBoard * 20 + position.nNum + (position.portA ? 0 : 8);
}

bool Gpio::setPin(const Info& info, PinPosition position, bool value)
{
    if (position.nBoard < info.boardCount && position.nBoard >= 0)
    {
        if (position.nNum < 0 || position.nNum >= 8)
        {
            std::cout << "Error: board index out of range (" << position.nNum << ")\n";
            return false;
        }

        bool type = position.portA
                    ? info.boards[position.nBoard].board_a[position.nNum].type
                    : info.boards[position.nBoard].board_b[position.nNum].type;

        if (type != OUTPUT)
        {
            std::cout << "Error: accessed pin (" << position.nNum
                      << ") is not set as OUTPUT\n";
            return false;
        }

        digitalWrite(Gpio::getPinIdx(position), value);
        return true;
    }

    std::cout << "Error: Invalid Board Number (" << position.nBoard << ")\n";
    return false;
}

std::array<bool, 2> readPin(PinPosition position, Info& info);
{
    std::array<bool, 2> result = { false, false };
    result[0] = true;

    if (position.nBoard < info.boardCount && position.nBoard >= 0)
    {
        if (position.nNum < 0 || position.nNum >= 8)
        {
            std::cout << "Error: board index out of range (" << position.nNum << ")\n";
            result[0] = false;
        }
        else
        {
            result[1] = digitalRead(Gpio::getPinIdx(position));

            if (position.portA)
                info.boards[position.nBoard].board_a[position.nNum].value = result[1];
            else
                info.boards[position.nBoard].board_b[position.nNum].value = result[1];
        }
    }
    else
    {
        std::cout << "Error: Invalid Board Number (" << position.nBoard << ")\n";
        result[0] = false;
    }

    return result;
}
