#pragma once

#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "BinaryTree.hpp"

class Pause{

    int pos;
    bool pressed, the_selected;

    BinaryTree boost_tree;

    sf::RectangleShape * winclose;
    sf::Font * font;
    sf::Texture * image;
    sf::Sprite * bg;
    sf::Sprite * bluecircle1;
    sf::Sprite * bluecircle2;
    sf::Sprite * bluecircle3;
    sf::Sprite * bluecircle4;
    sf::Sprite * bluecircle5;
    sf::Sprite * bluecircle6;
    sf::Sprite * bluecircle7;
    sf::Texture * image_circle;
    //std::vector<sf::Sprite*> bluecircle{7};
    sf::Texture * image_speed;
    sf::Texture * image_damage;
    sf::Sprite * speed1;
    sf::Sprite * damage2;
    sf::Sprite * damage3;
    sf::Sprite * speed4;
    sf::Sprite * speed5;
    sf::Sprite * damage6;
    sf::Sprite * damage7;

    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_cord;

    std::vector<const char *> options;
    std::vector<sf::Vector2f> coords;
    std::vector<sf::Vector2f> coords_bluecircle;
    std::vector<sf::Text> texts;
    std::vector<std::size_t> sizes;

protected:
    void setvalues();
    Game::PauseOption search_for_events(sf::RenderWindow&, sf::Event, Character&);
    void draw_all(sf::RenderWindow&);

public:
    Pause();
    ~Pause();
    Game::PauseOption RunPause(sf::RenderWindow&, sf::Event, Character&);
};