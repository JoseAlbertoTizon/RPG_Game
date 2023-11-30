#include "game.hpp"
#include "utility.hpp"
#include <filesystem>
#include <iostream>

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

void Game::load() {
    is_loading = true;
    std::fstream save_file;
    save_file.open(save_data);
    if(not save_file.is_open()) {
        is_loading = false;
        return;
    }

    save_data_map["character_standing_circle: "] = read_value_from_line(save_file);
    save_data_map["character_health: "] = read_value_from_line(save_file);
    save_data_map["character_coins: "] = read_value_from_line(save_file);
    save_data_map["number_of_enemies: "] = read_value_from_line(save_file);
    for(int i = 0; i < (int)save_data_map["number_of_enemies: "]; ++ i) {
        save_data_map["enemy_" + std::to_string(i) + "_position.x: "] = read_value_from_line(save_file);
        save_data_map["enemy_" + std::to_string(i) + "_position.y: "] = read_value_from_line(save_file);
        save_data_map["enemy_" + std::to_string(i) + "_health: "] = read_value_from_line(save_file);
    }
    bool edge_exists;
    for(int i = 0; i < 12; ++ i)
        for(int j = 0; j < 12; ++ j) {
        save_file >> edge_exists;
        save_data_map[std::to_string(i) + ", " + std::to_string(j) + ": "] = edge_exists;
    }
    int price;
    for(int i = 0; i < 12; ++ i) {
        save_file >> price;
        save_data_map["price_vertex_" + std::to_string(i) + ": "] = price;
    }
    save_data_map["game_difficulty: "] = read_value_from_line(save_file);
}

void Game::read_save_from(std::string save_file){
    save_data = save_file;
}



