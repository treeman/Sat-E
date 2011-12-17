#pragma once

#include "Item.hpp"

class Junk : public Item {
public:
    Junk( sf::Sprite spr );

    sf::IntRect BoundingBox();

    void Update( float dt );
    void Draw();
private:
    sf::Sprite spr;
};

