#pragma once

#include <SFML/Graphics.hpp>

float length(sf::Vector2f);

int random_int();

bool is_numeric(char c);

float read_value_from_line(std::fstream&);