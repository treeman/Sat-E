#include "Satellite.hpp"

Satellite::Satellite()
{
    pos = Vec2f( 100, 100 );

    main = BUTLER->CreateSprite( "gfx/basic.png" );
    main.SetCenter( 5, 5 );

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

    // Rotate a bit
    float angle = atan2( vel.y, vel.x ) - atan2( acc.y, acc.x ) + math::PI_2;

    float degree = 180 / math::PI * angle;

    main.Rotate( degree * dt );
}

void Satellite::Draw()
{
    main.SetPosition( pos );
    Tree::Draw( main );

    narrative.SetPos( pos );
    narrative.Draw();
}

