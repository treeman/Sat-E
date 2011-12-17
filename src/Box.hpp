#pragma once

#include "Tree.hpp"
#include "Item.hpp"

class Box : public Item {
public:
    Box();

    sf::IntRect BoundingBox();

    float MaxVel();

    void Update( float dt );
    void Draw();
private:
    sf::Sprite spr;
};

