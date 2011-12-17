#pragma once

#include "Tree.hpp"

struct Star {
    Vec2i pos;
    float power;
    Tree::Color color;
};

class Chunk {
public:
    Chunk( sf::IntRect rect );

    void Draw( Vec2i offset );

    Vec2i start_pos;

    typedef std::vector<Star> Stars;
    Stars stars;

    sf::Sprite star_spr;
};

