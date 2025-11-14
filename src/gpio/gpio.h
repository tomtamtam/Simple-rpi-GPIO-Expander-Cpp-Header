#pragma once

#include <vector>
#include <iostream>
#include <array>

namespace Gpio
{
    struct Pin
    {
        bool type;
        bool value;
    };

    struct PinPosition
    {
        int nBoard;
        bool portA;
        int nNum;

        PinPosition() = default;
        PinPosition(int b, bool a, int n) : nBoard(b), portA(a), nNum(n) {}
    };

    struct Board
    {
        Pin board_a[8];
        Pin board_b[8];
        unsigned int address;
    };

    struct Info
    {
        const std::string CONFIG_FILENAME = "config.json";
        int boardCount = 0;
        std::vector<Board> boards;
        Info();
    };

    void init(const Info& info);

    int getPinIdx(PinPosition position);

    bool setPin(const Info& info, PinPosition position, bool value);

    std::array<bool, 2> Gpio::readPin(Gpio::PinPosition position, Gpio::Info& info);
}
