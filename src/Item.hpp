#pragma once

#include "Tree.hpp"

class Item {
public:
    virtual ~Item() { }

    virtual sf::IntRect BoundingBox();

    void Accelerate( Vec2f acc );

    // Set speed
    virtual float MaxVel() = 0;

    virtual void Update( float dt );
    virtual void Draw() = 0;
protected:
    void UpdateMovement( float dt );

    Vec2f pos;
    Vec2f vel;
    Vec2f acc;
};

