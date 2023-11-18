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
    speed = 2;
    hitbox.x = 40;
    hitbox.y = 60;
    x_locked = false;
    y_locked = false;
};

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