#include "game.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

Game::Game() {
    window.setFramerateLimit(60);
}

void Game::loop() {
    sf::Clock elapsed_time;

    // Create background
    Background background("Background_Textures", "background_rpg_2");

    // Create main character
    Character character("Character_Sprites", "still_down");
    character.sprite.setPosition(0, 0);

    // Create skeleton vector
    std::vector<Enemy> skeletons;
    skeletons.push_back(Enemy{"Skeleton_Sprites", "still_down"});
    skeletons[0].sprite.setPosition(100, 100);

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(100, 100);

    int i = 0;

    // Game loop
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Key Pressing
            if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::W)


            // Key Releasing
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::W)
                character.change_texture("still_up");
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::S)
                character.change_texture("still_down");
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::A)
                character.change_texture("still_left");
            if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::D)
                character.change_texture("still_right");
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            character.move(0.f, -character.speed);
            background.move(0.f, character.speed);
            if(not background.y_locked)
                skeletons[0].move(0.f, character.speed);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_up_1": "walking_up_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            character.move(0.f, character.speed);
            background.move(0.f, -character.speed);
            if(not background.y_locked)
                skeletons[0].move(0.f, -character.speed);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_down_1": "walking_down_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
//            character.add_health(-1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            character.move(-character.speed, 0.f);
            background.move(character.speed, 0.f);
            if(not background.x_locked)
                skeletons[0].move(character.speed, 0.f);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_left_1": "walking_left_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            character.move(character.speed, 0.f);
            background.move(-character.speed, 0.f);
            if(not background.x_locked)
                skeletons[0].move(-character.speed, 0.f);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_right_1": "walking_right_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
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

        sf::Vector2f dist_vector = character.position() - skeletons[0].position();
        double distance = sqrt(dist_vector.x*dist_vector.x + dist_vector.y*dist_vector.y);
        if(distance < 40)
            character.add_health(-1);
        sf::Vector2f unit_vector;
        unit_vector.x = dist_vector.x / distance;
        unit_vector.y = dist_vector.y / distance;
        skeletons[0].move(skeletons[0].speed*unit_vector.x, skeletons[0].speed*unit_vector.y);
        if(skeletons[0].position().y > character.position().y) {
            if ((int) (elapsed_time.getElapsedTime().asSeconds() / Character::sprite_time) % 2 == 0)
                skeletons[0].change_texture((i % 2 == 0) ? "skell_walking_up_1" : "skell_walking_up_2");
        }
        else if ((int) (elapsed_time.getElapsedTime().asSeconds() / Character::sprite_time) % 2 == 0)
            skeletons[0].change_texture((i % 2 == 0) ? "skell_walking_down_1" : "skell_walking_down_2");




        window.clear();

        window.draw(background.sprite);
        window.draw(circle);
        window.draw(character.sprite);
        window.draw(skeletons[0].sprite);
        window.draw(character.health_bar[0]);
        window.draw(character.health_bar[1]);
        window.draw(character.text);
        window.display();

    }
}