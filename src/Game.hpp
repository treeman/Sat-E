#pragma once

#include "Tree.hpp"
#include "Satellite.hpp"

class Game : public Tree::GameState {
public:
    Game();

    bool HandleEvent( sf::Event &e );

    void Update( float dt );
    void Draw();
private:
    sf::Sprite background;

    Satellite satellite;
};

