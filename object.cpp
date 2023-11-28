#include "object.hpp"
#include "utility.hpp"
#include "game.hpp"

Object::Object(std::map<std::string, sf::Texture>& textures, const std::string& default_texture) {
    change_texture(textures, default_texture);
}

Object::Object() = default;

Character::Character(std::map<std::string, sf::Texture>& textures, const std::string& default_texture)
    : Object(textures, default_texture) {

    // Set initial values
    font.loadFromFile("ARIAL.ttf");
    speed = 2;

    coin.change_texture(textures, "coin");
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

Enemy::Enemy(std::map<std::string, sf::Texture>& textures, const std::string& default_texture)
        : Object(textures, default_texture) {

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

Background::Background(std::map<std::string, sf::Texture>& textures, const std::string& default_texture)
        : Object(textures, default_texture) {

    // Set initial values
};

void Object::change_texture(std::map<std::string, sf::Texture>& textures, const std::string& texture_name) {
    sprite.setTexture(textures[texture_name]);
}

void Object::move(float delta_x, float delta_y) {
    sprite.setPosition(position() + sf::Vector2f(delta_x, delta_y));
}

void Object::move_to(float x, float y) {
    sf::Vector2f dist_vector = sf::Vector2f{x, y} - this->position();
    move_to_direction(dist_vector);
}

void Object::move_to_direction(sf::Vector2f dist_vector) {
    float distance = length(dist_vector);
    sf::Vector2f unit_vector = {dist_vector.x / distance, dist_vector.y / distance};
    sf::Vector2f displacement = {this->speed * unit_vector.x, this->speed * unit_vector.y};
    this->move(displacement.x, displacement.y);
}

sf::Vector2f Object::position() {
    return sprite.getPosition();
}

void Character::add_coins(int dif) {
    coins += dif;
    coin_text.setString(std::to_string(coins));
}

void Character::add_health(int dif) {
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

void Enemy::add_health(int dif) {
    if(health + dif >= max_health) {
        health = max_health;
        return;
    }
    health += dif;
    health_bar[0].setSize(sf::Vector2f(30*health/max_health, 10));
    health_bar[1].setSize(sf::Vector2f(30*(max_health-health)/max_health, 10));
    health_bar[1].setPosition(health_bar[0].getPosition() + sf::Vector2f(health_bar[0].getSize().x, 0));
}

void Character::render(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(health_bar[0]);
    window.draw(health_bar[1]);
    window.draw(coin.sprite);
    window.draw(heart.sprite);
    window.draw(text);
    window.draw(coin_text);
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(health_bar[0]);
    window.draw(health_bar[1]);
}

void Background::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

