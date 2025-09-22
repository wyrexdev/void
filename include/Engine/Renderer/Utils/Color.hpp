#include <string>
#include <iostream>
#include <iomanip>

class Color {
public:
    void setColor(float r, float g, float b, float a);
    void setColor(std::string color);

    std::string getColor();

private:
    std::string color;
};