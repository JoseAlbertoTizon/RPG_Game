#include "game.hpp"
#include <iostream>
#include <filesystem>

Game::Game() {
    window.setFramerateLimit(60);
}

float abs(sf::Vector2f vector) {
    return sqrt(vector.x*vector.x + vector.y*vector.y);
}

void load_all_textures() {
    for(const auto& texture_file: std::filesystem::directory_iterator("textures")) {
        std::string texture_name_png = texture_file.path().filename().string();
        std::string texture_name = texture_name_png.substr(0, texture_name_png.size() - 4);
        if(not Game::textures.count(texture_name_png))
            Game::textures[texture_name].loadFromFile("textures/" + texture_name_png);
    }
}

void Game::loop() {
    load_all_textures();

    sf::Clock elapsed_time;
    sf::Clock mob_elapsed_time;

    // Create background
    Background background("background_rpg_2");

    // Create graph for movement
    Graph graph;
    graph.add_edge(0, 1);
    graph.add_edge(1, 0);

//    std::vector<int> path = graph.find_minimum_path(0, 1);
//    for(int x: path)
//        std:: cout << x << "\n";

    // Create main character
    Character character("still_down");
    character.sprite.setPosition(100+8, 100-10);

    // Create skeleton vector
    std::vector<Enemy> skeletons;
    skeletons.push_back(Enemy{"skel_still_down"});
    skeletons.push_back(Enemy{"skel_still_down"});
    skeletons[0].sprite.setPosition(100, 100);
    skeletons[1].sprite.setPosition(400, 200);
    for (auto &skeleton: skeletons)
        skeleton.sprite.setScale(1.5, 1.5);

    int i = 0, j = 0;

    // Game loop
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (int k = 0; k < graph.vertices.size(); ++ k) {
                    if (graph.vertices[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y) and not character.is_moving) {
                        character.path = graph.find_minimum_path(character.from_circle, k);
                        for(auto x: character.path)
                            std::cout << x << "\n";
                        if(character.path.empty())
                            std::cout << "no path available\n";
                        if(not character.path.empty() and *(character.path.end()) != character.from_circle) {
                            character.is_moving = true;
                            character.to_circle = character.path[1];
                        }
                        break;
                    }
                }
            }
            if(event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                for (int k = 0; k < graph.vertices.size(); ++ k) {
                    if (graph.vertices[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        graph.add_edge(character.from_circle, k);
                        graph.add_edge(k, character.from_circle);
                    }
                }
            }
        }

        if(character.is_moving) {
            sf::Vector2f to_position = graph.vertices[character.to_circle].getPosition();
            sf::Vector2f direction = to_position - character.position();

            if (abs(direction) > 15) {
                character.move_to(to_position.x + 8, to_position.y - 10);
                float distance = abs(direction);
                sf::Vector2f unit_vector = {direction.x / distance, direction.y / distance};
                sf::Vector2f displacement = {character.speed * unit_vector.x, character.speed * unit_vector.y};
                background.move(-displacement.x, -displacement.y);

                for (auto &skeleton: skeletons) {
                    if (not background.x_locked)
                        skeleton.move(-displacement.x, 0);
                    if (not background.y_locked)
                        skeleton.move(0, -displacement.y);
                }
                for (auto &circle: graph.vertices) {
                    if (not background.x_locked)
                        circle.move(-displacement.x, 0);
                    if (not background.y_locked)
                        circle.move(0, -displacement.y);
                }
                for(auto& line: graph.lines) {
                    if (not background.x_locked) {
                        line[0].position = line[0].position + sf::Vector2f(-displacement.x, 0);
                        line[1].position = line[1].position + sf::Vector2f(-displacement.x, 0);
                        line[2].position = line[2].position + sf::Vector2f(-displacement.x, 0);
                        line[3].position = line[3].position + sf::Vector2f(-displacement.x, 0);

                    }
                    if (not background.y_locked) {
                        line[0].position = line[0].position + sf::Vector2f(0, -displacement.y);
                        line[1].position = line[1].position + sf::Vector2f(0, -displacement.y);
                        line[2].position = line[2].position + sf::Vector2f(0, -displacement.y);
                        line[3].position = line[3].position + sf::Vector2f(0, -displacement.y);
                    }
                }

                if (not background.x_locked)
                    to_position += sf::Vector2f{-displacement.x, 0};
                if (not background.y_locked)
                    to_position += sf::Vector2f{0, -displacement.y};

                if (direction.y > 0) {
                    if (elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
                        character.change_texture((i % 2 == 0) ? "walking_down_1" : "walking_down_2");
                        i = (i + 1) % 2;
                        elapsed_time.restart();
                    }
                }
                else if (elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
                    character.change_texture((i % 2 == 0) ? "walking_up_1" : "walking_up_2");
                    i = (i + 1) % 2;
                    elapsed_time.restart();
                }
            }
            else {
                character.path.erase(character.path.begin());
                character.from_circle = character.path[0];
                if (not character.path.empty())
                    character.to_circle = character.path[1];
                else {
                    character.is_moving = false;
                    character.change_texture("still_down");
                }
            }
        }

        if(character.position().x - background.position().x < window.getSize().x / 2.0) {
            character.x_locked = false;
            background.x_locked = true;
            background.sprite.setPosition(0, background.position().y);
        }
        else if(background.position().x + background.size().x - character.position().x < window.getSize().x / 2.0) {
            character.x_locked = false;
            background.x_locked = true;
            background.sprite.setPosition((float)window.getSize().x - background.size().x, background.position().y);
        }
        else {
            character.x_locked = true;
            background.x_locked = false;
        }
        if(character.position().y - background.position().y < window.getSize().y / 2.0) {
            character.y_locked = false;
            background.y_locked = true;
            background.sprite.setPosition(background.position().x, 0);
        }
        else if(background.position().y + background.size().y - character.position().y < window.getSize().y / 2.0) {
            character.y_locked = false;
            background.y_locked = true;
            background.sprite.setPosition(background.position().x, (float)window.getSize().y - background.size().y);
        }
        else {
            character.y_locked = true;
            background.y_locked = false;
        }

        for(auto& skeleton: skeletons) {
            sf::Vector2f dist_vector = character.position() - skeleton.position();
            float distance = abs(dist_vector);
            if (distance < 40)
                character.add_health(1);
            skeleton.move_to(character.position().x, character.position().y);
            skeleton.health_bar[0].setPosition(skeleton.position() + sf::Vector2f(33, 10));
            skeleton.health_bar[1].setPosition(skeleton.health_bar[0].getPosition() + sf::Vector2f(skeleton.health_bar[0].getSize().x, 0));
            if (skeleton.position().y > character.position().y) {
                if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15)
                    skeleton.change_texture((j % 2 == 0) ? "skel_walking_up_1" : "skel_walking_up_2");
            }
            else if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15)
                skeleton.change_texture((j % 2 == 0) ? "skel_walking_down_1" : "skel_walking_down_2");
        }
        if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
            j = (j + 1) % 2;
            mob_elapsed_time.restart();
        }


        window.clear();

        window.draw(background.sprite);
        for(auto& line: graph.lines)
            window.draw(line);
        for(auto& circle: graph.vertices)
            window.draw(circle);
        window.draw(character.sprite);
        for(auto& skeleton: skeletons) {
            window.draw(skeleton.sprite);
            window.draw(skeleton.health_bar[0]);
            window.draw(skeleton.health_bar[1]);
        }
        window.draw(character.coin.sprite);
        window.draw(character.coin_text);
        window.draw(character.heart.sprite);
        window.draw(character.health_bar[0]);
        window.draw(character.health_bar[1]);
        window.draw(character.text);
        window.display();

    }
}