#include "Satellite.hpp"

Satellite::Satellite()
{
    pos = Vec2f( 100, 100 );

    main = BUTLER->CreateSprite( "gfx/basic.png" );
    move_snd = BUTLER->CreateSound( "snd/burst1.wav" );
    move_snd.SetLoop( true );
}

sf::IntRect Satellite::BoundingBox()
{
    return sf::IntRect( pos.x, pos.y, pos.x + 10, pos.y + 10 );
}

float Satellite::MaxVel()
{
    return TWEAKS->GetNum( "satellite_max_vel" );
}

void Satellite::Update( float dt )
{
    if( acc != Vec2f::zero && move_snd.GetStatus() != sf::Sound::Playing ) {
        move_snd.Play();
    }
    else if( acc == Vec2f::zero ) {
        move_snd.Stop();
    }

    UpdateMovement( dt );

    Vec2f comp( 0, 1 );
    float angle = atan2( vel.y, vel.x ) - atan2( comp.y, comp.x ) + math::PI;

    float degree = angle * 180 / math::PI;
    main.SetRotation( degree );

    std::stringstream ss;
    ss << vel << " => " << degree;
    Tree::VisualDebug( "angle", ss.str() );
}

void Satellite::Draw()
{
    main.SetPosition( pos );
    Tree::Draw( main );
}

