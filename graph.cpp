#include <SFML/Graphics.hpp>
#include "graph.hpp"
#include "utility.hpp"
#include <set>
#include <limits>

Graph::Graph() {
    for(auto& circle: vertices) {
        circle.setFillColor(sf::Color::Red);
        circle.setOutlineColor(sf::Color::Black);
        circle.setRadius(15);
    }

    vertices[0].setPosition(100, 100);
    vertices[1].setPosition(200, 200);
    vertices[2].setPosition(200, 300);
    vertices[3].setPosition(100, 400);
    vertices[4].setPosition(100, 500);
    vertices[5].setPosition(200, 500);
    vertices[6].setPosition(300, 500);
    vertices[7].setPosition(400, 400);
    vertices[8].setPosition(500, 300);
    vertices[9].setPosition(500, 400);
    vertices[10].setPosition(1000, 100);
    vertices[11].setPosition(1000, 100);
    vertices[12].setPosition(1000, 100);

    for(int i = 0; i < 12; ++ i)
        for(int j = 0; j < 12; ++ j) {
            edges[i][j] = false;
            weights[i][j] = length(vertices[i].getPosition() - vertices[j].getPosition());
        }

    font.loadFromFile("ARIAL.ttf");

    for(int i = 0; i < 12; ++ i) {
        price_texts[i] = sf::Text{std::to_string(prices[i]), font};
        price_texts[i].setFillColor(sf::Color::White);
        price_texts[i].setPosition(vertices[i].getPosition() + sf::Vector2f(5, 5));
        price_texts[i].setCharacterSize(15);
    }
}

void Graph::add_edge(int from, int to) {
    edges[from][to] = true;
    sf::VertexArray line(sf::TriangleStrip, 4);
    line[0].position = vertices[from].getPosition() + sf::Vector2f(15, 15);
    line[1].position = vertices[to].getPosition() + sf::Vector2f(15, 15);;
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;

    sf::Vector2f direction = line[0].position - line[1].position;
    sf::Vector2f unit_normal_direction = {-direction.y / length(direction), direction.x / length(direction)};

    line[0].position = line[0].position - sf::Vector2f(unit_normal_direction.x*1, unit_normal_direction.y*1);
    line[1].position = line[1].position - sf::Vector2f(unit_normal_direction.x*1, unit_normal_direction.y*1);
    line[2].position = line[0].position + sf::Vector2f(unit_normal_direction.x*1, unit_normal_direction.y*1);
    line[3].position = line[1].position + sf::Vector2f(unit_normal_direction.x*1, unit_normal_direction.y*1);
    line[2].color = sf::Color::Red;
    line[3].color = sf::Color::Red;

    lines.push_back(line);

    prices[to] = 2 * prices[to] + 5;
    price_texts[to].setString(std::to_string(prices[to]));
}

std::vector<int> Graph::find_minimum_path(int from, int to) {
    std::set<int> unvisited;
    float distances[12];
    int previous[12];
    for(int i = 0; i < 12; ++ i){
        distances[i] = std::numeric_limits<float>::max();
        previous[i] = -1;
    }
    distances[from] = 0;
    for(int i = 0; i < 12; ++ i)
        unvisited.insert(i);

    while(not unvisited.empty()){
        int current;
        float minimum_distance = std::numeric_limits<float>::max();
        for(int i: unvisited)
            if(distances[i] <= minimum_distance) {
                minimum_distance = distances[i];
                current = i;
            }
        unvisited.erase(current);
        for(int i = 0; i < 12; ++ i){
            if(edges[current][i])
                if(distances[current] + weights[current][i] < distances[i]) {
                    distances[i] = distances[current] + weights[current][i];
                    previous[i] = current;
                }
        }
    }

    std::vector<int> path;
    while(to != -1) {
        path.push_back(to);
        to = previous[to];
    }
    std::reverse(path.begin(), path.end());
    if(path[0] != from)
        return {};
    return path;
}

void Graph::render(sf::RenderWindow& window) {
    for(auto& line: lines)
        window.draw(line);
    for(auto& circle: vertices)
        window.draw(circle);
    for(auto& price_text: price_texts)
        window.draw(price_text);
}
