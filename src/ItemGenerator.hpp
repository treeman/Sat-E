#pragma once

#include "Tree.hpp"

#include "Junk.hpp"

class Star {
public:
    Star( Vec2i _pos, Tree::Color _color );

    Vec2i GetPos() { return pos; }

    void Move( Vec2i offset );
    void Draw( Vec2i offset );
private:
    Vec2i pos;
    float power;
    Tree::Color color;

    sf::Sprite spr;
};

class ItemGenerator {
public:
    ItemGenerator( int chunk_w, int chunk_h );

    ItemPtr CreateJunk( Vec2i offset = Vec2i::zero );
    ItemPtr CreateAsteroid( Vec2i offset = Vec2i::zero );

    Star CreateStar( Vec2i offset = Vec2i::zero );
private:
    Tree::ShuffleBag<sf::Sprite> junk_bag;
    Tree::ShuffleBag<sf::Sprite> asteroid_bag;

    Tree::ShuffleBag<Vec2i> position_bag;

    const int chunk_w, chunk_h;

    void Fill( Tree::ShuffleBag<sf::Sprite> &bag, std::string img, int w, int h );
};

