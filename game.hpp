#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "object.hpp"

class Game {
public:
    enum GameState{MENU, PAUSE, RUNNING, CREDITS, DEAD, WON};

    enum MenuOption{Nothing, NewGame, Continue, Credits};

    enum PauseOption{KeepPaused, Unpause, Save, Exit, AddSpeed, AddDamage};

    Game();

    void read_save_from(std::string);

    void loop();

private:
    GameState gameState = MENU;
    MenuOption menuOption;
    PauseOption pauseOption;

    sf::RenderWindow window{sf::VideoMode(670, 670), "RPG"};

    std::map<std::string, sf::Texture> textures;
    void load_all_textures();

    bool released_left = false;
    bool released_right = false;

    float difficulty = 0;
    void spawn_enemy(std::vector<Enemy>&);

    std::map<std::string, float> save_data_map;
    std::string save_data;

    bool is_loading = false;
    void load();
};


