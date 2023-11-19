#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <cmath>

class Game {
public:
    Game();
    void loop();

//private:
    sf::RenderWindow window{sf::VideoMode(640, 640), "RPG"};
    static std::map<std::string, sf::Texture> textures;

    class Object {
    public:
        explicit Object(const std::string&, const std::string&);
        void change_texture(const std::string&);
        void move(float, float);
        void move_to(float, float);
        sf::Vector2f size();
        sf::Vector2f position();

        sf::Sprite sprite;
        sf::Vector2f hitbox;
        float speed;
        bool x_locked, y_locked;
        constexpr static const float sprite_time = 0.15;
    };

    class Character : public Object {
    public:
        sf::Font font;
        sf::Text text;
        Character(const std::string&, const std::string&);
        int health = 200, max_health = 200;
        std::vector<sf::RectangleShape> health_bar;
        void add_health(int);
        sf::Vector2f moving_to = {100+8, 100-10};
    };

    class Enemy : public Object {
    public:
        Enemy(const std::string&, const std::string&);
        int health = 200, max_health = 200;
        std::vector<sf::RectangleShape> health_bar;
        void add_health(int);
    };

    class Obstacle : public Object {
    public:
        Obstacle(const std::string&, const std::string&);
    };

    class Background : public Object {
    public:
        Background(const std::string&, const std::string&);
    };

    class Graph {
    public:
        Graph();
        std::vector<std::vector<bool>> edges {20, std::vector<bool>(20, false)};
        std::vector<sf::CircleShape> vertices {20};
        void add_edge(int, int);
    };

};

