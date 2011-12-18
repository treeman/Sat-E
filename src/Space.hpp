#pragma once

#include <boost/shared_ptr.hpp>

#include "Satellite.hpp"
#include "Box.hpp"
#include "Chunk.hpp"
#include "ItemGenerator.hpp"
#include "Dock.hpp"

class Space {
public:
    Space();

    void HandleEvent( sf::Event &e );

    void Update( float dt );
    void Draw();
private:
    Satellite satellite;

    std::vector<double> star_colors;

    // Our base box ^^
    Box box;

    // Check space so we're in an initialized zone
    void UpdateChunks( float dt );

    void UpdateChunk( int x, int y, float dt );
    void UpdateChunk( Vec2i chunk, float dt );

    // Allocate space objects inside rect
    void AllocateChunk( Vec2i chunk_index );
    void AllocateChunk( int x, int y );

    // Return the current space chunk we're viewing
    Vec2i CurrentChunkIndex();
    sf::IntRect ChunkRect( Vec2i chunk );

    // Chunks we've allocated
    std::set<Vec2i> existing_chunks;
    // Chunks we've generated surroundings for
    std::set<Vec2i> checked_chunks;
    // Chunks collecting all stuff inside them
    typedef std::map<Vec2i, Chunk> Chunks;
    Chunks chunks;

    void DrawChunk( int x, int y, Vec2i offset );
    void DrawChunk( Vec2i chunk, Vec2i offset );

    void CenterCam( Vec2i pos );
    Vec2i cam;

    ItemGenerator generator;

    void Intersects( ItemPtr item );

    sf::Sprite life_spr;

    void DrawLife();

    void JunkAdded( int num );
    int junk_collected;

    sf::Sprite junk_spr;
    sf::String junk_str;
    Tree::ShuffleBag<sf::Sound> junk_snd;
    sf::Sound curr_junk_snd; // Workaround ^^

    void DrawJunk();

    sf::Sprite fuel_spr;

    void DrawFuel();

    // Docking menu
    // Might find a better place for this later
    // but then we'd have to create a new wrapper class maybe with gui this menu and space
    // ... and that's a lot of code atm
    // this will do for now
    Dock dock;

    bool can_activate_docking;

    sf::Sprite arrow_home_spr;

    void Teleport();

    sf::Sound hit_snd;
    sf::Sound heal_snd;
};

