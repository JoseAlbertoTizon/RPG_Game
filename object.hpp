#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>

class Object {
public:
    explicit Object(std::map<std::string, sf::Texture>&, const std::string&);
    Object();

    void change_texture(std::map<std::string, sf::Texture>&, const std::string&);

    void move(float, float);
    void move_to(float, float);
    void move_to_direction(sf::Vector2f);

    sf::Vector2f position();

    sf::Vector2f destination_direction;

    sf::Sprite sprite;
    float speed;

    bool is_hidden = false;

};

class Character : public Object {
public:
    Character(std::map<std::string, sf::Texture>&, const std::string&);

    sf::Font font;
    sf::Text text, coin_text;

    Object coin;
    int coins = 0;
    void add_coins(int);

    int damage = 5;

    std::vector<sf::RectangleShape> health_bar;
    Object heart;
    int health = 200, max_health = 200;
    void add_health(int);

    int from_circle = 0;
    int to_circle;
    bool is_moving = false;
    std::vector<int> path{};

    void render(sf::RenderWindow& window);

    void save_state_to_file(std::fstream&);
};

class Enemy : public Object {
public:
    Enemy(std::map<std::string, sf::Texture>&, const std::string&);

    int health = 200, max_health = 200;
    std::vector<sf::RectangleShape> health_bar;
    void add_health(int);

    void render(sf::RenderWindow& window);

    void save_state_to_file(std::fstream&);
};

class Background : public Object {
public:
    Background(std::map<std::string, sf::Texture>&, const std::string&);

    void render(sf::RenderWindow& window);
};