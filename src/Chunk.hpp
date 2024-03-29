#pragma once

#include "Tree.hpp"
#include "ItemGenerator.hpp"

class Chunk {
public:
    Chunk( sf::IntRect rect, ItemGenerator generator, int distance );

    Items GetItems();

    float Rand() { return rand; }

    void Update( float dt );
    void Draw( Vec2i offset, sf::IntRect visible );
private:
    Vec2i start_pos;

    // Stars
    typedef std::vector<Star> Stars;
    Stars stars;

    // And no stripes
    Items items;

    // Item generator
    ItemGenerator &generator;

    float rand;
};

