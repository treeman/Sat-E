#pragma once

#include "Tree.hpp"

#include "Item.hpp"
#include "Narrative.hpp"

class Satellite : public Item {
public:
    Satellite();

    sf::IntRect BoundingBox();

    float MaxVel();

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sprite main;

    sf::Sound move_snd;

    Narrative narrative;
};

