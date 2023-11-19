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
    health_bar[0].setPosition(420, 20);
    health_bar[1].setPosition(420 + health_bar[0].getSize().x, 20);
    text.setString(std::to_string(health) + "/" + std::to_string(max_health));
}
