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

    // Create main character
    Character character("Character_Sprites", "still_down");
    character.sprite.setPosition(0, 0);
    sf::RectangleShape health_bar;
    health_bar.setSize(sf::Vector2f(200, 20));
    health_bar.setOutlineColor(sf::Color::Black);
    health_bar.setFillColor(sf::Color::Red);
    health_bar.setOutlineThickness(3);
    health_bar.setPosition(420, 20);
    sf::Font font;
    font.loadFromFile("ARIAL.ttf");
    sf::Text text("200/200", font);
    text.setFillColor(sf::Color::Black);
    text.setPosition(49, 20);
    text.setCharacterSize(15);


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

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_up_1": "walking_up_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            character.move(0.f, character.speed);
            background.move(0.f, -character.speed);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_down_1": "walking_down_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            character.move(-character.speed, 0.f);
            background.move(character.speed, 0.f);

            if(elapsed_time.getElapsedTime().asSeconds() >= Character::sprite_time) {
                character.change_texture((i % 2 == 0)? "walking_left_1": "walking_left_2");
                i = (i + 1) % 2;
                elapsed_time.restart();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            character.move(character.speed, 0.f);
            background.move(-character.speed, 0.f);

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


        window.clear();
        window.draw(background.sprite);
        window.draw(character.sprite);
        window.draw(health_bar);
        window.draw(text);
        window.display();

    }
}