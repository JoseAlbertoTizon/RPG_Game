#pragma once

#include <SFML/Graphics.hpp>
#include "game.hpp"

class Menu{

    int pos;
    bool pressed, the_selected;

    sf::RectangleShape * winclose;
    sf::Font * font;
    sf::Texture * image;
    sf::Sprite * bg;

    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_cord;

    std::vector<const char *> options;
    std::vector<sf::Vector2f> coords;
    std::vector<sf::Text> texts;
    std::vector<std::size_t> sizes;

protected:
    void setvalues();
    Game::MenuOption search_for_events(sf::RenderWindow&, sf::Event);
    void draw_all(sf::RenderWindow&);

public:
    Menu();
    ~Menu();
    Game::MenuOption RunMenu(sf::RenderWindow&, sf::Event);

};