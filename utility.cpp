#include "utility.hpp"
#include <cmath>
#include <ctime>


float length(sf::Vector2f vector) {
    return sqrt(vector.x*vector.x + vector.y*vector.y);
}

int random_int() {
    std::srand(std::time(0));
    return std::rand();
}