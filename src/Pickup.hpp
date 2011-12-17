#pragma once

#include "Tree.hpp"

class Pickup {
public:
    virtual ~Pickup() { }

    void SetPos( Vec2f _pos ) { pos = _pos; }

    virtual sf::IntRect BoundingBox() = 0;

    virtual void Update( float dt ) { }
    virtual void Draw() = 0;
protected:
    Vec2f pos;
};

