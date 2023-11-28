#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"

class Graph {
public:
    Graph();

    bool edges[12][12];
    float weights[12][12];

    std::vector<int> prices{0, 3, 6, 10, 20, 30, 50, 70, 90, 120, 150, 200};

    sf::Font font;
    std::vector<sf::Text> price_texts {12};

    std::vector<sf::CircleShape> vertices {12};
    std::vector<sf::VertexArray> lines;

    void add_edge(int, int);
    std::vector<int> find_minimum_path(int, int);

    void render(sf::RenderWindow& window);
};