#include "game.hpp"
#include <filesystem>

std::map<std::string, sf::Texture> Game::textures;

Game::Object::Object(const std::string& texture_folder, const std::string& default_texture) {
    // Load textures
    for(const auto& texture_file: std::filesystem::directory_iterator(texture_folder)) {
        std::string texture_name_png = texture_file.path().filename().string();
        std::string texture_name = texture_name_png.substr(0, texture_name_png.size() - 4);
        if(not Game::textures.count(texture_name_png))
            Game::textures[texture_name].loadFromFile(texture_folder + "/" + texture_name_png);
    }
    change_texture(default_texture);
}

Game::Character::Character(const std::string& texture_folder, const std::string& default_texture)
    : Object(texture_folder, default_texture) {
    // Set initial values
    font.loadFromFile("ARIAL.ttf");
    speed = 2;
    hitbox.x = 40;
    hitbox.y = 60;
    x_locked = false;
    y_locked = false;
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

Game::Enemy::Enemy(const std::string& texture_folder, const std::string& default_texture)
        : Object(texture_folder, default_texture) {
    // Set initial values
    speed = 1;
    hitbox.x = 40;
    hitbox.y = 60;
    x_locked = false;
    y_locked = false;
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(30, 10)));
    health_bar.push_back(sf::RectangleShape(sf::Vector2f(0, 10)));
    health_bar[0].setOutlineColor(sf::Color::Black);
    health_bar[1].setOutlineColor(sf::Color::Black);
    health_bar[0].setFillColor(sf::Color::Red);
    health_bar[1].setFillColor(sf::Color::Black);
    health_bar[0].setOutlineThickness(1);
    health_bar[1].setOutlineThickness(1);
    health_bar[0].setPosition(this->position() + sf::Vector2f(0, -15));
    health_bar[1].setPosition(health_bar[0].getPosition() + sf::Vector2f(health_bar[0].getSize().x, 0));

}

Game::Obstacle::Obstacle(const std::string& texture_folder, const std::string& default_texture)
        : Object(texture_folder, default_texture) {
    // Set initial values
    x_locked = true;
    y_locked = true;
};

Game::Background::Background(const std::string& texture_folder, const std::string& default_texture)
        : Object(texture_folder, default_texture) {
    // Set initial values
    x_locked = true;
    y_locked = true;
};

void Game::Object::change_texture(const std::string& texture_name) {
    sprite.setTexture(textures[texture_name]);
}

void Game::Object::move(float delta_x, float delta_y) {
    if(not x_locked)
        sprite.setPosition(position().x + delta_x, position().y);
    if(not y_locked)
        sprite.setPosition(position().x, position().y + delta_y);
}

void Game::Object::move_to(float x, float y) {
    sf::Vector2f dist_vector = sf::Vector2f{x, y} - this->position();
    float distance = sqrt(dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y);
    sf::Vector2f unit_vector = {dist_vector.x / distance, dist_vector.y / distance};
    sf::Vector2f displacement = {this->speed * unit_vector.x, this->speed * unit_vector.y};
    if(not x_locked)
        this->move(displacement.x, 0);
    if(not y_locked)
        this->move(0, displacement.y);
}

sf::Vector2f Game::Object::position() {
    return sprite.getPosition();
}

sf::Vector2f Game::Object::size() {
    return sprite.getGlobalBounds().getSize();
}

void Game::Character::add_health(int dif) {
    if(health + dif >= max_health) {
        health = max_health;
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
        circle.setFillColor(sf::Color::White);
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
    vertices[13].setPosition(100, 100);
    vertices[14].setPosition(100, 100);
    vertices[15].setPosition(100, 100);
    vertices[16].setPosition(100, 100);
    vertices[17].setPosition(100, 100);
    vertices[18].setPosition(100, 100);
    vertices[19].setPosition(100, 100);
    vertices[20].setPosition(100, 100);

}

void Game::Graph::add_edge(int from, int to) {
    edges[from][to] = true;
    edges[to][from] = true;
}