#pragma once

#include "Item.hpp"

class Asteroid : public Item {
public:
    Asteroid( sf::Sprite spr );

    sf::IntRect BoundingBox();

    float MaxVel();

    ItemBehavior Behavior();

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sprite spr;
};

