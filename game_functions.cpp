#include "game.hpp"
#include "utility.hpp"
#include <filesystem>
#include <fstream>
#include <cctype>

Game::Game() {
    window.setFramerateLimit(60);
}

void Game::load_all_textures() {
    for(const auto& texture_file: std::filesystem::directory_iterator("textures")) {
        std::string texture_name_png = texture_file.path().filename().string();
        std::string texture_name = texture_name_png.substr(0, texture_name_png.size() - 4);
        if(not textures.count(texture_name_png))
            textures[texture_name].loadFromFile("textures/" + texture_name_png);
    }
}

void Game::spawn_enemy(std::vector<Enemy>& enemies) {
    Enemy enemy(textures, "skel_still_down");

    int choice = random_int() % 4;
    switch(choice) {
        case(0):
            enemy.sprite.setPosition(335, 0 - 10);
            enemies.push_back(enemy);
            break;

        case(1):
            enemy.sprite.setPosition(670 + 10, -335);
            enemies.push_back(enemy);
            break;

        case(2):
            enemy.sprite.setPosition(335, 670 + 10);
            enemies.push_back(enemy);
            break;

        case(3):
            enemy.sprite.setPosition(0 - 10, 335);
            enemies.push_back(enemy);
            break;

    }
}

//void Game::load_save_file(const std::string& save_file) {
//    std::ifstream save;
//    save.open(save_file);
//
//    char aux;
//    float value;
//    while(save >> aux) {
//        if(save.peek())
//    }
//}

