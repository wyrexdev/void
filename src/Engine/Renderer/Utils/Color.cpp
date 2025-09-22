#include "Engine/Renderer/Utils/Color.hpp"

void Color::setColor(float r, float g, float b, float a)
{
    int hexR = static_cast<int>(r * 255);
    int hexG = static_cast<int>(g * 255);
    int hexB = static_cast<int>(b * 255);
    int hexA = static_cast<int>(a * 255);

    std::stringstream ss;
    ss << "#"
       << std::setw(2) << std::setfill('0') << std::hex << hexR
       << std::setw(2) << std::setfill('0') << std::hex << hexG
       << std::setw(2) << std::setfill('0') << std::hex << hexB
       << std::setw(2) << std::setfill('0') << std::hex << hexA;

    color = ss.str();
}

std::string Color::getColor() {
    return color;
}