#include "Junk.hpp"

Junk::Junk( sf::Sprite _spr ) : spr(_spr)
{
}

sf::IntRect Junk::BoundingBox()
{
    return sf::IntRect( pos.x -5, pos.y -5, pos.x + 5, pos.y + 5 );
}

ItemBehavior Junk::Behavior()
{
    return AddJunk;
}

void Junk::Update( float dt )
{
    spr.Rotate( TWEAKS->GetNum( "junk_rotate" ) * dt );
}

void Junk::Draw( Vec2i offset )
{
    spr.SetPosition( pos + offset );
    Tree::Draw( spr );
}

