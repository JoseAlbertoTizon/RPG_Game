#include "utility.hpp"
#include <cmath>
#include <ctime>
#include <fstream>

float length(sf::Vector2f vector) {
    return sqrt(vector.x*vector.x + vector.y*vector.y);
}

int random_int() {
    std::srand(std::time(0));
    return std::rand();
}

bool is_numeric(char c) {
    return c >= '0' and c <= '9';
}

float read_value_from_line(std::fstream& file) {
    char buffer;
    float value;
    while(file >> buffer and buffer != ':');
    file >> value;
    return value;
}
