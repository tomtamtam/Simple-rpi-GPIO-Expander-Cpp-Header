#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include "json.h"
#include "./../gpio/gpio.h"

using json = nlohmann::json;

std::string Json::parse(std::string filename)
{
    std::ifstream File(filename);
    if (!File) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
        return "";
    }
    
    std::string res;
    std::string line;
    while (std::getline(File, line)) {
        res += line + "\n";
    }
    File.close();
    return res;
}

std::pair<int, std::vector<Gpio::Board>> Json::parseToBoards(std::string filename)
{
    std::string fileContent = parse(filename);
    
    json j = json::parse(fileContent);
    
    const int boardCount = j["board_count"];
    std::vector<Gpio::Board> boards;
    
    for(int n = 0; n < boardCount; n++)
    {
        Gpio::Board b;
        
        json boardData = j["boards"][n];
        
        std::string addressStr = boardData["address"];
        b.address = std::stoul(addressStr, nullptr, 16);
        
        for(int i = 0; i < 8; i++)
        {
            b.board_a[i].type = boardData["a"]["types"][i];
            b.board_a[i].value = boardData["a"]["values"][i];
        }
        
        for(int i = 0; i < 8; i++)
        {
            b.board_b[i].type = boardData["b"]["types"][i];
            b.board_b[i].value = boardData["b"]["values"][i];
        }
        
        boards.push_back(b);
    }
    
    return {boardCount, boards};
}
