#include "game.hpp"
#include <filesystem>
#include <limits>
#include <set>

std::map<std::string, sf::Texture> Game::textures;

Game::Object::Object(const std::string& default_texture) {
    change_texture(default_texture);
}

Game::Character::Character(const std::string& default_texture)
    : Object(default_texture) {
    // Set initial values
    font.loadFromFile("ARIAL.ttf");
    speed = 2;

    coin.sprite.setScale(0.04, 0.04);
    coin.sprite.setPosition(389.7, 53);
    coin_text = sf::Text{"0", font};
    coin_text.setFillColor(sf::Color::White);
    coin_text.setPosition(420, 53);
    coin_text.setCharacterSize(15);
    text.setFillColor(sf::Color::White);
    text.setPosition(430, 53);
    text.setCharacterSize(15);
    heart.sprite.setPosition(390, 22);
    heart.sprite.setScale(0.01,0.01);
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(200, 20)));
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(0, 20)));
    health_bar[0].setOutlineColor(sf::Color::Black);
    health_bar[1].setOutlineColor(sf::Color::Black);
    health_bar[0].setFillColor(sf::Color::Red);
    health_bar[1].setFillColor(sf::Color::Black);
    health_bar[0].setOutlineThickness(3);
    health_bar[1].setOutlineThickness(3);
    health_bar[0].setPosition(420, 20);
    health_bar[1].setPosition(620, 20);
    text = sf::Text{"200/200", font};
    text.setFillColor(sf::Color::White);
    text.setPosition(490, 20);
    text.setCharacterSize(15);
};

Game::Enemy::Enemy(const std::string& default_texture)
        : Object(default_texture) {
    // Set initial values
    speed = 1;
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(30, 10)));
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(0, 10)));
    health_bar[0].setOutlineColor(sf::Color::Black);
    health_bar[1].setOutlineColor(sf::Color::Black);
    health_bar[0].setFillColor(sf::Color::Red);
    health_bar[1].setFillColor(sf::Color::Black);
    health_bar[0].setOutlineThickness(1);
    health_bar[1].setOutlineThickness(1);
    health_bar[0].setPosition(this->position() + sf::Vector2f(33, 10));
    health_bar[1].setPosition(health_bar[0].getPosition() + sf::Vector2f(health_bar[0].getSize().x, 0));

}

Game::Obstacle::Obstacle(const std::string& default_texture)
        : Object(default_texture) {
    // Set initial values
};

Game::Background::Background(const std::string& default_texture)
        : Object(default_texture) {
    // Set initial values
};

void Game::Object::change_texture(const std::string& texture_name) {
    sprite.setTexture(textures[texture_name]);
}

void Game::Object::move(float delta_x, float delta_y) {
    sprite.setPosition(position() + sf::Vector2f(delta_x, delta_y));
}

void Game::Object::move_to(float x, float y) {
    sf::Vector2f dist_vector = sf::Vector2f{x, y} - this->position();
    float distance = sqrt(dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y);
    sf::Vector2f unit_vector = {dist_vector.x / distance, dist_vector.y / distance};
    sf::Vector2f displacement = {this->speed * unit_vector.x, this->speed * unit_vector.y};
    this->move(displacement.x, displacement.y);
}

sf::Vector2f Game::Object::position() {
    return sprite.getPosition();
}

sf::Vector2f Game::Object::size() {
    return sprite.getGlobalBounds().getSize();
}

void Game::Character::add_coins(int dif) {
    coins += dif;
    coin_text.setString(std::to_string(coins));
}

void Game::Character::add_health(int dif) {
    if(health + dif >= max_health) {
        health = max_health;
        return;
    }
    if(health + dif <= 0) {
        health = 0;
        return;
    }

    health += dif;
    health_bar[0].setSize(sf::Vector2f(200*health/max_health, 20));
    health_bar[1].setSize(sf::Vector2f(200*(max_health-health)/max_health, 20));
    health_bar[1].setPosition(health_bar[0].getPosition() + sf::Vector2f(health_bar[0].getSize().x, 0));
    text.setString(std::to_string(health) + "/" + std::to_string(max_health));
}

void Game::Enemy::add_health(int dif) {
    if(health + dif >= max_health) {
        health = max_health;
        return;
    }
    health += dif;
    health_bar[0].setSize(sf::Vector2f(30*health/max_health, 10));
    health_bar[1].setSize(sf::Vector2f(30*(max_health-health)/max_health, 10));
    health_bar[1].setPosition(health_bar[0].getPosition() + sf::Vector2f(health_bar[0].getSize().x, 0));
}

Game::Graph::Graph() {
    for(auto& circle: vertices) {
        circle.setFillColor(sf::Color::Red);
        circle.setOutlineColor(sf::Color::Black);
        circle.setRadius(30);
    }

    vertices[0].setPosition(100, 100);
    vertices[1].setPosition(300, 300);
    vertices[2].setPosition(600, 600);
    vertices[3].setPosition(600, 300);
    vertices[4].setPosition(100, 100);
    vertices[5].setPosition(100, 100);
    vertices[6].setPosition(100, 100);
    vertices[7].setPosition(100, 100);
    vertices[8].setPosition(100, 100);
    vertices[9].setPosition(100, 100);
    vertices[10].setPosition(100, 100);
    vertices[11].setPosition(100, 100);
    vertices[12].setPosition(100, 100);

    for(int i = 0; i < 12; ++ i)
        for(int j = 0; j < 12; ++ j) {
            edges[i][j] = false;
            weights[i][j] = abs(vertices[i].getPosition() - vertices[j].getPosition());
        }
}

void Game::Graph::add_edge(int from, int to) {
    edges[from][to] = true;
    sf::VertexArray line(sf::TriangleStrip, 4);
    line[0].position = vertices[from].getPosition() + sf::Vector2f(15, 15);
    line[1].position = vertices[to].getPosition() + sf::Vector2f(15, 15);;
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;

    sf::Vector2f direction = line[0].position - line[1].position;
    sf::Vector2f unit_normal_direction = {-direction.y / abs(direction), direction.x / abs(direction)};

    line[0].position = line[0].position - sf::Vector2f(unit_normal_direction.x*2.5, unit_normal_direction.y*2.5);
    line[1].position = line[1].position - sf::Vector2f(unit_normal_direction.x*2.5, unit_normal_direction.y*2.5);
    line[2].position = line[0].position + sf::Vector2f(unit_normal_direction.x*2.5, unit_normal_direction.y*2.5);
    line[3].position = line[1].position + sf::Vector2f(unit_normal_direction.x*2.5, unit_normal_direction.y*2.5);
    line[2].color = sf::Color::Red;
    line[3].color = sf::Color::Red;

    lines.push_back(line);
}

std::vector<int> Game::Graph::find_minimum_path(int from, int to) {
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