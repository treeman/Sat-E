#include "Box.hpp"

Box::Box()
{
    pos = Vec2f( 400, 300 );
    spr = BUTLER->CreateSprite( "box" );
    spr.SetCenter( 10, 10 );
}

sf::IntRect Box::BoundingBox()
{
    return sf::IntRect( pos.x, pos.y, pos.x + 20, pos.y + 20 );
}

float Box::MaxVel()
{
    return 0;
}

void Box::Update( float dt )
{
    UpdateMovement( dt );
    spr.Rotate( TWEAKS->GetNum( "box_rotate" ) * dt );
}

void Box::Draw()
{
    spr.SetPosition( pos );
    Tree::Draw( spr );
}

