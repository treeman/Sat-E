#include "Powerup.hpp"

Powerup::Powerup()
{
    spr = BUTLER->CreateSprite( "power" );
}

sf::IntRect Powerup::BoundingBox()
{
    return sf::IntRect( pos.x, pos.y, pos.x + 20, pos.y + 20 );
}

void Powerup::Draw()
{
    spr.SetPosition( pos );
    Tree::Draw( spr );
}

