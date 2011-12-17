#include "Box.hpp"

Box::Box() : grid_visible( false )
{
    pos = Vec2f( 400, 300 );
    spr = BUTLER->CreateSprite( "box" );
    spr.SetCenter( 10, 10 );

    grid = BUTLER->CreateSprite( "gfx/grid.png" );
    grid.SetCenter( 28, 28 );
    grid.SetColor( Tree::Color( 0x66189E13 ) );
}

sf::IntRect Box::BoundingBox()
{
    return sf::IntRect( pos.x, pos.y, pos.x + 20, pos.y + 20 );
}

float Box::MaxVel()
{
    return 0;
}

void Box::IsClose( bool b )
{
    grid_visible = b;
}

void Box::Update( float dt )
{
    UpdateMovement( dt );
    spr.Rotate( TWEAKS->GetNum( "box_rotate" ) * dt );
    grid.Rotate( TWEAKS->GetNum( "box_rotate" ) * dt );
}

void Box::Draw( Vec2i offset )
{
    grid.SetPosition( pos + offset );
    spr.SetPosition( pos + offset );
    if( grid_visible ) Tree::Draw( grid );
    Tree::Draw( spr );
}

