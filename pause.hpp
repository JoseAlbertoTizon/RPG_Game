#pragma once

#include <SFML/Graphics.hpp>
#include "game.hpp"

class Pause{

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
    Game::PauseOption search_for_events(sf::RenderWindow&, sf::Event);
    void draw_all(sf::RenderWindow&);

public:
    Pause();
    ~Pause();
    Game::PauseOption RunPause(sf::RenderWindow&, sf::Event);
};