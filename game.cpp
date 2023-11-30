#include "game.hpp"
#include "object.hpp"
#include "graph.hpp"
#include "utility.hpp"
#include "menu.hpp"
#include "pause.hpp"
#include <cmath>
#include <iostream>

void Game::loop() {
    load_all_textures();

    // Create menu interface
    Menu menu;

    // Create pause interface
    Pause pause;

    // Death screen
    sf::Font font;
    font.loadFromFile("SNAP____.TTF");
    sf::RectangleShape close_button{sf::Vector2f(150, 60)};
    sf::RectangleShape go_to_save_button{sf::Vector2f(150, 60)};
    close_button.setPosition(140, 400);
    go_to_save_button.setPosition(380, 400);
    sf::Text close_text{"Close", font};
    close_text.setCharacterSize(25);
    sf::Text go_to_save_text{"Last Save", font};
    go_to_save_text.setCharacterSize(25);
    close_text.setPosition(160, 415);
    go_to_save_text.setPosition(380, 410);
    close_text.setFillColor(sf::Color::Black);
    go_to_save_text.setFillColor(sf::Color::Black);
    close_text.setOutlineColor(sf::Color::Red);
    go_to_save_text.setOutlineColor(sf::Color::Red);

    // Credits screen
    sf::Font arial_font;
    arial_font.loadFromFile("ARIAL.ttf");
    sf::Text credits_text{"Game Producers:\n\n\nJose Alberto Feijao Tizon \n\n Eduardo Santos Guerra \n\n Italo dos Santos Rabelo", arial_font};
    credits_text.setPosition(200, 150);
    credits_text.setCharacterSize(20);
    credits_text.setFillColor(sf::Color::White);

    // Create game clocks
    sf::Clock elapsed_time, mob_elapsed_time, spawn_clock, wave_clock;

    // Create save file
    std::fstream save_file;

    // Create background
    Background background(textures, "level_1");

    // Create graph for movement
    Graph graph;

    // Create main character
    Character character(textures, "still_down");
    character.sprite.setPosition(graph.vertices[character.from_circle].getPosition() + sf::Vector2f(0, -10));
    character.sprite.setScale(0.75, 0.75);

    // Create skeleton vector
    std::vector<Enemy> skeletons;

    // Create projectiles vector;
    std::vector<Object> projectiles;

    // Wave text;
    sf::Text wave;
    wave.setFont(font);
    wave.setCharacterSize(20);
    wave.setPosition(30, 30);
    wave.setString("Wave: " + std::to_string(1));

    int i = 0, j = 0;

    // Game loop
    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameState == RUNNING) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    for (int k = 0; k < graph.vertices.size(); ++k) {
                        if (graph.vertices[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y) and
                            not character.is_moving) {
                            character.path = graph.find_minimum_path(character.from_circle, k);
                            if (not character.path.empty() and *(character.path.end() - 1) != character.from_circle) {
                                character.is_moving = true;
                                character.to_circle = character.path[1];
                            }
                            break;
                        } else if (released_left) {
                            Object projectile(textures, "bullet");
                            projectile.sprite.setPosition(character.position());
                            projectile.sprite.setScale(0.02, 0.02);
                            projectile.destination_direction = (sf::Vector2f) mousePosition - projectile.position();
                            projectile.speed = 10;
                            projectiles.push_back(projectile);
                            released_left = false;
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right and released_right and not character.is_moving) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    for (int k = 0; k < graph.vertices.size(); ++k) {
                        if (graph.vertices[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            if (not graph.edges[character.from_circle][k] and character.coins >= graph.prices[k]) {
                                character.add_coins(-graph.prices[k]);
                                graph.add_edge(character.from_circle, k);
                            }
                        }
                    }
                    released_right = false;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    released_left = true;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Right)
                    released_right = true;
            }

            if (character.health < 0)
                gameState = DEAD;

            if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Down and gameState != DEAD)
                gameState = PAUSE;

            if(is_loading) {
                character.from_circle = (int)save_data_map["character_standing_circle: "];
                character.sprite.setPosition(graph.vertices[character.from_circle].getPosition() + sf::Vector2f(0, -10));
                character.health = (int)save_data_map["character_health: "];
                character.add_health(0);
                character.coins = (int)save_data_map["character_coins: "];
                character.add_coins(0);
                while(not skeletons.empty())
                    skeletons.pop_back();
                for(int k = 0; k < (int)save_data_map["number_of_enemies: "]; ++ k) {
                    Enemy skeleton(textures, "skel_still_down");
                    skeleton.sprite.setPosition(save_data_map["enemy_" + std::to_string(k) + "_position.x: "], save_data_map["enemy_" + std::to_string(k) + "_position.y: "]);
                    skeleton.health = (int)save_data_map["enemy_" + std::to_string(k) + "_health: "];
                    skeleton.add_health(0);
                    skeletons.push_back(skeleton);
                }
                for(int i = 0; i < 12; ++ i)
                    for(int j = 0; j < 12; ++ j) {
                        if ((int) save_data_map[std::to_string(i) + ", " + std::to_string(j) + ": "] == 1)
                            graph.add_edge(i, j);
                    }
                for(int i = 0; i < 12; ++ i) {
                    graph.prices[i] = (int)save_data_map["price_vertex_" + std::to_string(i) + ": "];
                    graph.price_texts[i].setString(std::to_string(graph.prices[i]));
                }

                difficulty = save_data_map["game_difficulty: "];
                std::cout << difficulty << "\n";
                wave.setString("Wave: " + std::to_string((int)(1 + difficulty)));

                is_loading = false;
            }

            if (character.is_moving) {
                sf::Vector2f to_position = graph.vertices[character.to_circle].getPosition();
                sf::Vector2f direction = to_position - character.position();

                if (length(direction) > 15) {
                    character.move_to(to_position.x + 8, to_position.y - 10);
                    if (direction.y > 0) {
                        if (elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
                            character.change_texture(textures, (i % 2 == 0) ? "walking_down_1" : "walking_down_2");
                            i = (i + 1) % 2;
                            elapsed_time.restart();
                        }
                    } else if (elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
                        character.change_texture(textures, (i % 2 == 0) ? "walking_up_1" : "walking_up_2");
                        i = (i + 1) % 2;
                        elapsed_time.restart();
                    }
                } else {
                    character.sprite.setPosition(graph.vertices[character.to_circle].getPosition() + sf::Vector2f(0, -10));
                    character.path.erase(character.path.begin());
                    character.from_circle = character.path[0];
                    if (not character.path.empty())
                        character.to_circle = character.path[1];
                    else {
                        character.is_moving = false;
                        character.change_texture(textures, "still_down");
                    }
                }
            }

            for (int k = 0; k < skeletons.size(); ++k) {
                auto &skeleton = skeletons[k];
                sf::Vector2f dist_vector = character.position() - skeleton.position();
                if (length(dist_vector) < 40)
                    character.add_health(-0);
                for (auto &projectile: projectiles) {
                    dist_vector = projectile.position() - skeleton.position();
                    if (length(dist_vector) < 60)
                        skeleton.add_health(-character.damage);
                }

                skeleton.move_to(character.position().x, character.position().y);
                skeleton.health_bar[0].setPosition(skeleton.position() + sf::Vector2f(18, 16));
                skeleton.health_bar[1].setPosition(skeleton.health_bar[0].getPosition() + sf::Vector2f(skeleton.health_bar[0].getSize().x, 0));
                if (skeleton.position().y > character.position().y) {
                    if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15)
                        skeleton.change_texture(textures, (j % 2 == 0) ? "skel_walking_up_1" : "skel_walking_up_2");
                } else if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15)
                    skeleton.change_texture(textures, (j % 2 == 0) ? "skel_walking_down_1" : "skel_walking_down_2");

                if (skeleton.health < 0) {
                    skeletons.erase(skeletons.begin() + k);
                    character.add_coins(10);
                }

            }
            if (mob_elapsed_time.getElapsedTime().asSeconds() >= 0.15) {
                j = (j + 1) % 2;
                mob_elapsed_time.restart();
            }

            for (int k = 0; k < projectiles.size(); ++k) {
                auto &projectile = projectiles[k];
                projectile.move_to_direction(projectile.destination_direction);
                if (fabs(projectile.position().x) > 1000 or fabs(projectile.position().y) > 1000 or
                    projectile.position().x < -100 or projectile.position().y < -100)
                    projectiles.erase(projectiles.begin() + k);
            }

            if(wave_clock.getElapsedTime().asSeconds() >= 12) {
                difficulty ++;
                wave.setString("Wave: " + std::to_string((int)(1 + difficulty)));
                if(difficulty == 10)
                    gameState = WON;
                wave_clock.restart();
            }
            if (spawn_clock.getElapsedTime().asSeconds() >= 5 - difficulty / 2) {
                spawn_enemy(skeletons);
                spawn_clock.restart();
            }
            for (auto &skeleton: skeletons)
                skeleton.speed = 1 + difficulty / 5;

            window.clear();

            background.render(window);
            graph.render(window);
            character.render(window);
            for (auto &skeleton: skeletons)
                skeleton.render(window);
            for (auto &projectile: projectiles)
                window.draw(projectile.sprite);
            window.draw(wave);

            window.display();
        }

        if (gameState == MENU) {
            menuOption = menu.RunMenu(window, event);
            if (menuOption == NewGame) {
                gameState = RUNNING;
                wave_clock.restart();
                is_loading = false;
            }
            if(menuOption == Continue) {
                gameState = RUNNING;
                load("save_data.txt");
            }
            if(menuOption == Credits) {
                gameState = CREDITS;
            }
        }

        if(gameState == CREDITS) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (close_button.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        gameState = MENU;
                }
            }
            window.clear();

            window.draw(close_button);
            window.draw(close_text);
            window.draw(credits_text);

            window.display();
        }

        if(gameState == PAUSE) {
            pauseOption = pause.RunPause(window, event);
            if(pauseOption == Unpause)
                gameState = RUNNING;
            if(pauseOption == Save) {
                gameState = RUNNING;
                save_file.close();
                save_file.open("save_data.txt", std::fstream::out | std::fstream::trunc);
                character.save_state_to_file(save_file);
                save_file << "number_of_enemies: " << skeletons.size() << "\n";
                for(auto& skeleton: skeletons)
                    skeleton.save_state_to_file(save_file);
                graph.save_to_file(save_file);
                save_file << "game_difficulty: " << difficulty << "\n";
            }
            if(pauseOption == Exit) {
                window.close();
            }
            if(pauseOption == AddSpeed) {
                character.speed *= 1.2;
            }
            if(pauseOption == AddDamage) {
                character.damage += 5;
            }
        }

        if (gameState == DEAD) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (close_button.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        window.close();
                    if(go_to_save_button.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        gameState = RUNNING;
                        load("save_data.txt");
                    }
                }
            }

            window.clear();

            window.draw(close_button);
            window.draw(go_to_save_button);
            window.draw(close_text);
            window.draw(go_to_save_text);

            window.display();
        }

        if(gameState == WON) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (close_button.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                        window.close();
                }
            }

            window.clear();

            window.draw(close_button);
            window.draw(close_text);

            window.display();
        }

    }
}