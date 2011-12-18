#include "Asteroid.hpp"

Asteroid::Asteroid( sf::Sprite _spr ) : spr(_spr)
{
    const float max = 10;
    vel = Vec2i( math::frandom( -max, max ), math::frandom( -max, max ) );
}

sf::IntRect Asteroid::BoundingBox()
{
    return sf::IntRect( pos.x -5, pos.y -5, pos.x + 5, pos.y + 5 );
}

float Asteroid::MaxVel()
{
    return 100;
}

ItemBehavior Asteroid::Behavior()
{
    return Hurts;
}

void Asteroid::Update( float dt )
{
    UpdateMovement( dt );
    spr.Rotate( math::frandom( 0.5, 1.0 ) * dt );
}

void Asteroid::Draw( Vec2i offset )
{
    spr.SetPosition( pos + offset );
    Tree::Draw( spr );
}

