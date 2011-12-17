#pragma once

#include <boost/shared_ptr.hpp>

#include "Satellite.hpp"
#include "Junk.hpp"
#include "Box.hpp"
#include "Chunk.hpp"
#include "ItemGenerator.hpp"

class Space {
public:
    Space();

    void Update( float dt );
    void Draw();
private:
    Satellite satellite;

    std::vector<double> star_colors;

    void DrawOutline( sf::IntRect box );

    // Our base box ^^
    Box box;

    // Check space so we're in an initialized zone
    void UpdateSpaceChunks();

    // Allocate space objects inside rect
    void AllocateChunk( Vec2i chunk_index );

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

    ItemGenerator generator;
};

