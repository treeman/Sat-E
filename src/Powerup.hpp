#pragma once

#include "Pickup.hpp"

class Powerup : public Pickup {
public:
    Powerup();

    sf::IntRect BoundingBox();

    void Draw();
private:
    sf::Sprite spr;
};

