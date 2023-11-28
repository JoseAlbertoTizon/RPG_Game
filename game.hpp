#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "object.hpp"

class Game {
public:
    Game();

    void loop();

//private:
    sf::RenderWindow window{sf::VideoMode(670, 670), "RPG"};

    std::map<std::string, sf::Texture> textures;
    void load_all_textures();

    bool released_left = false;
    bool released_right = false;

    float difficulty = 0;
    void spawn_enemy(std::vector<Enemy>&);
};


