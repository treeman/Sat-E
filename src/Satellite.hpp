#pragma once

#include "Tree.hpp"

class Satellite {
public:
    Satellite();

    void Accelerate( Vec2f acc );

    void Update( float dt );
    void Draw();
private:
    sf::Sprite main;

    Vec2f pos;
    Vec2f vel;
    Vec2f acc;
};

