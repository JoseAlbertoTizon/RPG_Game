#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "object.hpp"

class Game {
public:
    Game();

    void loop();

//private:
    enum GameState{MENU, RUNNING, DEAD};
    GameState gameState = MENU;

    enum MenuOption{Nothing, NewGame, Continue, Credits};
    MenuOption menuOption;

    sf::RenderWindow window{sf::VideoMode(670, 670), "RPG"};

    std::map<std::string, sf::Texture> textures;
    void load_all_textures();

    bool released_left = false;
    bool released_right = false;

    float difficulty = 0;
    void spawn_enemy(std::vector<Enemy>&);

    std::map<std::string, float> save_data;
    void load_save_file(const std::string&);
};


