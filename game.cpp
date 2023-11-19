#include "game.hpp"
#include <algorithm>
#include <iostream>

Game::Game() {
    window.setFramerateLimit(60);
}

void Game::loop() {
    sf::Clock elapsed_time;

    // Create background
    Background background("Background_Textures", "background_rpg_2");

    // Create graph for movement
    Graph graph;

    // Create main character
    Character character("Character_Sprites", "still_down");
    character.sprite.setPosition(100+8, 100-10);

    // Create skeleton vector
    std::vector<Enemy> skeletons;
    skeletons.push_back(Enemy{"Skeleton_Sprites", "skel_still_down"});
    skeletons.push_back(Enemy{"Skeleton_Sprites", "skel_still_down"});
    skeletons[0].sprite.setPosition(100, 100);
    skeletons[1].sprite.setPosition(400, 200);

    int i = 0;

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

                for (auto &circle: graph.vertices) {
                    if (circle.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        character.moving_to = {circle.getPosition().x, circle.getPosition().y};
                        break;
                    }
                }
            }
        }

        sf::Vector2f direction = character.moving_to - character.position();
        std::cout << sqrt(direction.x * direction.x + direction.y * direction.y) << "\n";
        if(sqrt(direction.x * direction.x + direction.y * direction.y) > 15) {
            character.move_to(character.moving_to.x + 8, character.moving_to.y - 10);
            sf::Vector2f dist_vector = character.moving_to - character.position();
            float distance = sqrt(dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y);
            sf::Vector2f unit_vector = {dist_vector.x / distance, dist_vector.y / distance};
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
            if(not background.x_locked)
                character.moving_to += sf::Vector2f{-displacement.x, 0};
            if(not background.y_locked)
                character.moving_to += sf::Vector2f{0, -displacement.y};

            if (direction.y > 0) {
                if (elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                    character.change_texture((i % 2 == 0) ? "walking_down_1" : "walking_down_2");
                    i = (i + 1) % 2;
                    elapsed_time.restart();
                }
            } else if (elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0) ? "walking_up_1" : "walking_up_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        else character.change_texture("still_down");

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
            float distance = sqrt(dist_vector.x * dist_vector.x + dist_vector.y * dist_vector.y);
            if (distance < 40)
                character.add_health(-1);
            skeleton.move_to(character.position().x, character.position().y);
            skeleton.health_bar[0].setPosition(skeleton.position() + sf::Vector2f(0, -15));
            skeleton.health_bar[1].setPosition(skeleton.health_bar[0].getPosition() + sf::Vector2f(skeleton.health_bar[0].getSize().x, 0));
            if (skeleton.position().y > character.position().y) {
                if ((int) (elapsed_time.getElapsedTime().asSeconds() / Character::sprite_time) % 2 == 0)
                    skeleton.change_texture((i % 2 == 0) ? "skel_walking_up_1" : "skel_walking_up_2");
            } else if ((int) (elapsed_time.getElapsedTime().asSeconds() / Character::sprite_time) % 2 == 0)
                skeleton.change_texture((i % 2 == 0) ? "skel_walking_down_1" : "skel_walking_down_2");
        }


        window.clear();

        window.draw(background.sprite);
        for(auto& circle: graph.vertices)
            window.draw(circle);
        window.draw(character.sprite);
        for(auto& skeleton: skeletons) {
            window.draw(skeleton.sprite);
            window.draw(skeleton.health_bar[0]);
            window.draw(skeleton.health_bar[1]);
        }
        window.draw(character.health_bar[0]);
        window.draw(character.health_bar[1]);
        window.draw(character.text);
        window.display();

    }
}