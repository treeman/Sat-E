#pragma once

#include "Tree.hpp"
#include "Item.hpp"

class Box : public Item {
public:
    Box();

    sf::IntRect BoundingBox();

    float MaxVel();

    void IsClose( bool b );

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sprite spr;
    sf::Sprite grid;
    bool grid_visible;
};

