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
        explicit Object(const std::string&);
        void change_texture(const std::string&);
        void move(float, float);
        void move_to(float, float);
        sf::Vector2f size();
        sf::Vector2f position();

        sf::Sprite sprite;
        float speed;
        bool x_locked, y_locked;
    };

    class Character : public Object {
    public:
        Character(const std::string&);

        sf::Font font;
        sf::Text text, coin_text;

        std::vector<Object> projectiles;
        Object coin{"coin"};
        int coins;
        void add_coins(int);

        std::vector<sf::RectangleShape> health_bar;
        Object heart{"heart"};
        int health = 200, max_health = 200;
        void add_health(int);

        int from_circle = 0;
        int to_circle;
        bool is_moving = false;
        std::vector<int> path{};
    };

    class Enemy : public Object {
    public:
        Enemy(const std::string&);

        int health = 200, max_health = 200;
        std::vector<sf::RectangleShape> health_bar;
        void add_health(int);
    };

    class Obstacle : public Object {
    public:
        Obstacle(const std::string&);
    };

    class Background : public Object {
    public:
        Background(const std::string&);
    };

    class Graph {
    public:
        Graph();
        bool edges[12][12];
        float weights[12][12];
        std::vector<int> prices{0, 1, 2, 5, 7, 9, 10, 20, 22, 24, 26, 30};
        std::vector<sf::CircleShape> vertices {12};
        std::vector<sf::VertexArray> lines;
        void add_edge(int, int);
        std::vector<int> find_minimum_path(int, int);
    };

};

float abs(sf::Vector2f);
void load_all_textures();


