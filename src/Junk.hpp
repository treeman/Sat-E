#pragma once

#include "Item.hpp"

class Junk : public Item {
public:
    Junk( sf::Sprite spr );

    sf::IntRect BoundingBox();

    ItemBehavior Behavior();

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sprite spr;
};

