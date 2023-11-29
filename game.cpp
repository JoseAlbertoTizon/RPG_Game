#include "game.hpp"
#include "object.hpp"
#include "graph.hpp"
#include "utility.hpp"
#include "menu.hpp"
#include <cmath>
#include <iostream>

void Game::loop() {
    load_all_textures();

    //

    // Create menu
    Menu menu;

    // Death screen


    // Create game clocks
    sf::Clock elapsed_time;
    sf::Clock mob_elapsed_time;
    sf::Clock spawn_clock;

    // Create background
    Background background(textures, "level_1");

    // Create graph for movement
    Graph graph;

    // Create main character
    Character character(textures, "still_down");
    character.sprite.setPosition(100+8, 100-10);
    character.sprite.setScale(0.75, 0.75);

    // Create skeleton vector
    std::vector<Enemy> skeletons;
//    skeletons.push_back(Enemy{textures, "skel_still_down"});
//    skeletons.push_back(Enemy{textures, "skel_still_down"});
//    skeletons[0].sprite.setPosition(100, 100);
//    skeletons[1].sprite.setPosition(400, 200);

    // Create projectiles vector;
    std::vector<Object> projectiles;

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
                        if (graph.vertices[k].getGlobalBounds().contains(mousePosition.x, mousePosition.y) and not character.is_moving) {
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
                                graph.add_edge(k, character.from_circle);
                                if (difficulty < 10)
                                    difficulty += 0.5;

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
                    character.add_health(-1);
                for (auto &projectile: projectiles) {
                    dist_vector = projectile.position() - skeleton.position();
                    if (length(dist_vector) < 60)
                        skeleton.add_health(-5);
                }

                skeleton.move_to(character.position().x, character.position().y);
                skeleton.health_bar[0].setPosition(skeleton.position() + sf::Vector2f(18, 16));
                skeleton.health_bar[1].setPosition(
                        skeleton.health_bar[0].getPosition() + sf::Vector2f(skeleton.health_bar[0].getSize().x, 0));
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

            if (spawn_clock.getElapsedTime().asSeconds() >= 5 - difficulty / 2.5) {
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

            window.display();
        }

        if(gameState == MENU) {
            menuOption = menu.RunMenu(window, event);
            if(menuOption == NewGame or menuOption == Continue)
                gameState = RUNNING;
        }
    }
}