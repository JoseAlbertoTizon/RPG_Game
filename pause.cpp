#include "pause.hpp"
#include <iostream>


Pause::Pause(){
    winclose = new sf::RectangleShape();
    font = new sf::Font();
    image = new sf::Texture();
    bg = new sf::Sprite();
//    for(int i = 0; i < 7; ++i)
//        bluecircle[i] = new sf::Sprite();
    bluecircle1 = new sf::Sprite();
    bluecircle2 = new sf::Sprite();
    bluecircle3 = new sf::Sprite();
    bluecircle4 = new sf::Sprite();
    bluecircle5 = new sf::Sprite();
    bluecircle6 = new sf::Sprite();
    bluecircle7 = new sf::Sprite();
    image_circle = new sf::Texture();

    setvalues();
};

Pause::~Pause(){
    delete winclose;
    delete font;
    delete image;
    delete bg;
//    for(int i = 0; i < 7; ++i)
//        delete bluecircle[i];
    delete bluecircle1;
    delete bluecircle2;
    delete bluecircle3;
    delete bluecircle4;
    delete bluecircle5;
    delete bluecircle6;
    delete bluecircle7;
    delete image_circle;
}

void Pause::setvalues() {
    pos = 0;
    pressed = false;
    the_selected = false;

    font->loadFromFile("SNAP____.TTF");
    image->loadFromFile("pausebackground.png");
    image_circle->loadFromFile("bluecircle.png");

    bg->setTexture(*image);

    pos_mouse = {0,0};
    mouse_cord = {0,0};

    options = {"Save" , "Quit"};
    texts.resize(2);
    coords = {{239, 447},{240, 552}};
    sizes = {45, 45};

    for(std::size_t i{}; i < texts.size(); ++i){
        texts[i].setFont(*font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(sizes[i]);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setPosition(coords[i]);

        texts[0].setOutlineThickness(8);

        winclose->setSize(sf::Vector2f(48,59));
        winclose->setPosition(1200,14);
        winclose->setFillColor(sf::Color::Black);
    }

//    coords_bluecircle = {{303,63},{227,170},{380,171},{143,273},{255,277},{345,276},{450,278}};
//    for(int i = 0; i < 9; ++i){
//        bluecircle[i]->setTexture(*image_circle);
//        bluecircle[i]->setPosition(coords_bluecircle[i]);
//        bluecircle[i]->setScale(0.05,0.05);
//    }

    bluecircle1->setTexture(*image_circle);
    bluecircle1->setPosition(303,63);
    bluecircle1->setScale(0.05,0.05);
    bluecircle2->setTexture(*image_circle);
    bluecircle2->setPosition(227,170);
    bluecircle2->setScale(0.05,0.05);
    bluecircle3->setTexture(*image_circle);
    bluecircle3->setPosition(380,171);
    bluecircle3->setScale(0.05,0.05);
    bluecircle4->setTexture(*image_circle);
    bluecircle4->setPosition(143,273);
    bluecircle4->setScale(0.05,0.05);
    bluecircle5->setTexture(*image_circle);
    bluecircle5->setPosition(255,277);
    bluecircle5->setScale(0.05,0.05);
    bluecircle6->setTexture(*image_circle);
    bluecircle6->setPosition(345,276);
    bluecircle6->setScale(0.05,0.05);
    bluecircle7->setTexture(*image_circle);
    bluecircle7->setPosition(450,278);
    bluecircle7->setScale(0.05,0.05);
}

Game::PauseOption Pause::search_for_events(sf::RenderWindow& window, sf::Event event) {
    pos_mouse = sf::Mouse::getPosition(window);
    mouse_cord = window.mapPixelToCoords(pos_mouse);

    if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Down and not pressed) {
        if (pos < 1) {
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
            return Game::Save;
        if (pos == 1)
            return Game::Exit;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (winclose->getGlobalBounds().contains(mouse_cord)) {
            window.close();
        }
        if(bluecircle1->getGlobalBounds().contains(mouse_cord) and not boost_tree.root->was_bought) {
            boost_tree.root->was_bought = true;
            bluecircle1->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->boost;
        }
        if(bluecircle2->getGlobalBounds().contains(mouse_cord) and boost_tree.root->was_bought and not boost_tree.root->lchild->was_bought) {
            boost_tree.root->lchild->was_bought = true;
            bluecircle2->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->lchild->boost;
        }
        if(bluecircle3->getGlobalBounds().contains(mouse_cord) and boost_tree.root->was_bought and not boost_tree.root->rchild->was_bought){
            boost_tree.root->rchild->was_bought = true;
            bluecircle3->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->rchild->boost;
        }
        if(bluecircle4->getGlobalBounds().contains(mouse_cord) and boost_tree.root->lchild->was_bought and not boost_tree.root->lchild->lchild->was_bought){
            boost_tree.root->lchild->lchild->was_bought = true;
            bluecircle4->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->lchild->lchild->boost;
        }
        if(bluecircle5->getGlobalBounds().contains(mouse_cord) and boost_tree.root->lchild->was_bought and not boost_tree.root->lchild->rchild->was_bought) {
            boost_tree.root->lchild->rchild->was_bought = true;
            bluecircle5->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->lchild->rchild->boost;
        }
        if(bluecircle6->getGlobalBounds().contains(mouse_cord) and boost_tree.root->rchild->was_bought and not boost_tree.root->rchild->lchild->was_bought){
            boost_tree.root->rchild->lchild->was_bought = true;
            bluecircle6->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->rchild->lchild->boost;
        }
        if(bluecircle7->getGlobalBounds().contains(mouse_cord) and boost_tree.root->rchild->was_bought and not boost_tree.root->rchild->rchild->was_bought){
            boost_tree.root->rchild->rchild->was_bought = true;
            bluecircle7->setColor(sf::Color::Green);
            boost_tree.LevelWalking();
            return boost_tree.root->rchild->rchild->boost;
        }
    }
    if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Right)
        return Game::Unpause;
    return Game::KeepPaused;
}

void Pause::draw_all(sf::RenderWindow& window) {
    window.clear();
    window.draw(*bg);
    for(const auto& t : texts){
        window.draw(t);
    }
    window.draw(*bluecircle1);
    window.draw(*bluecircle2);
    window.draw(*bluecircle3);
    window.draw(*bluecircle4);
    window.draw(*bluecircle5);
    window.draw(*bluecircle6);
    window.draw(*bluecircle7);

//    for(int i = 0; i < 7; ++i)
//        window.draw(*bluecircle[i]);
    window.display();
}

Game::PauseOption Pause::RunPause(sf::RenderWindow& window, sf::Event event) {
    draw_all(window);
    return search_for_events(window, event);
}

