//
// Created by eduardo on 26/11/2023.
//
#include <iostream>
#include "menu.hpp"

Menu::Menu(){
    winclose = new sf::RectangleShape();
    font = new sf::Font();
    image = new sf::Texture();
    bg = new sf::Sprite();

    setvalues();
};

Menu::~Menu(){
    delete winclose;
    delete font;
    delete image;
    delete bg;
}

void Menu::setvalues() {
    pos = 0;
    pressed = false;
    the_selected = false;

    font->loadFromFile("SNAP____.TTF");
    image->loadFromFile("menu_background.png");

    bg->setTexture(*image);

    pos_mouse = {0,0};
    mouse_cord = {0,0};

    options = {"New Game" , "Continue", "Credits"};
    texts.resize(3);
    coords = {{131,215},{135,369},{144, 511}};
    sizes = {50, 50, 50};

    for(std::size_t i{}; i < texts.size(); ++i){
        texts[i].setFont(*font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(sizes[i]);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setPosition(coords[i]);

        texts[1].setOutlineThickness(8);

        winclose->setSize(sf::Vector2f(48,59));
        winclose->setPosition(1200,14);
        winclose->setFillColor(sf::Color::Black);
    }
}

Game::MenuOption Menu::search_for_events(sf::RenderWindow& window, sf::Event event) {
    pos_mouse = sf::Mouse::getPosition(window);
    mouse_cord = window.mapPixelToCoords(pos_mouse);

    if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Down and not pressed) {
        if (pos < 2) {
            ++pos;
            pressed = true;
            texts[pos].setOutlineThickness(8);
            texts[pos - 1].setOutlineThickness(0);
            the_selected = false;
        }
    }
    if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Down)
        pressed = false;

    if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Up and not pressed) {
        if (pos > 0) {
            --pos;
            pressed = true;
            texts[pos].setOutlineThickness(8);
            texts[pos + 1].setOutlineThickness(0);
            the_selected = false;
        }
    }
    if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::Up)
        pressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) and not the_selected) {
        the_selected = true;
        if (pos == 0)
            return Game::NewGame;
        if (pos == 1)
            return Game::Continue;
        if (pos == 2)
            return Game::Credits;

    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (winclose->getGlobalBounds().contains(mouse_cord)) {
            window.close();
        }
    }
    return Game::Nothing;
}



void Menu::draw_all(sf::RenderWindow& window) {
    window.clear();
    window.draw(*bg);
    for(const auto& t : texts){
        window.draw(t);
    }
    window.display();
}

Game::MenuOption Menu::RunMenu(sf::RenderWindow& window, sf::Event event) {
    draw_all(window);
    return search_for_events(window, event);
}