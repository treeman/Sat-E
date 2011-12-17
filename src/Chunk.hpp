#pragma once

#include "Tree.hpp"
#include "ItemGenerator.hpp"

class Chunk {
public:
    Chunk( sf::IntRect rect, ItemGenerator generator );

    void Draw( Vec2i offset );
private:
    Vec2i start_pos;

    // Stars
    typedef std::vector<Star> Stars;
    Stars stars;

    // And no stripes
    typedef std::vector<boost::shared_ptr<Item> > Items;
    Items items;

    // Item generator
    ItemGenerator &generator;
};

