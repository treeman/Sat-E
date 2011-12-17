#pragma once

#include <boost/shared_ptr.hpp>

#include "Satellite.hpp"
#include "Junk.hpp"
#include "Box.hpp"
#include "Chunk.hpp"

class Space {
public:
    Space();

    void Update( float dt );
    void Draw();
private:
    Satellite satellite;

    typedef std::vector<Star> Stars;
    Stars stars;

    std::vector<double> star_colors;

    sf::Sprite star_spr;

    typedef std::vector<boost::shared_ptr<Item> > Items;
    Items items;

    void DrawOutline( sf::IntRect box );

    Box box;

    Tree::ShuffleBag<sf::Sprite> junk_bag;
    Tree::ShuffleBag<Vec2i> position_bag;

    void InitJunk();

    // Check space so we're in an initialized zone
    void UpdateSpaceChunks();

    // Allocate space objects inside rect
    void AllocateChunk( sf::IntRect rect );

    // Return the current space chunk we're viewing
    Vec2i CurrentChunkIndex();
    sf::IntRect ChunkRect( Vec2i chunk );

    // Chunks we've allocated
    std::set<Vec2i> existing_chunks;
    // Chunks collecting all stuff inside them
    typedef std::map<Vec2i, Chunk> Chunks;
    Chunks chunks;

    void DrawChunk( Vec2i chunk );

    void CenterCam( Vec2i pos );
    Vec2i cam;
};

