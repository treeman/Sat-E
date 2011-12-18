#include "Satellite.hpp"

Satellite::Satellite()
{
    pos = Vec2f( 100, 100 );

    main = BUTLER->CreateSprite( "gfx/basic.png" );
    main.SetCenter( 5, 5 );

    move_snd = BUTLER->CreateSound( "snd/burst1.wav" );
    move_snd.SetLoop( true );

    max_vel = TWEAKS->GetNum( "satellite_max_vel" );
    fuel = max_fuel = 100;
    life = max_life = 100;

    boost = TWEAKS->GetNum( "satellite_acc" );
}

sf::IntRect Satellite::BoundingBox()
{
    return sf::IntRect( pos.x -5, pos.y -5, pos.x + 5, pos.y + 5 );
}

void Satellite::ChangeFuel( float mod )
{
    fuel += mod;
    if( fuel < 0 ) fuel = 0;
    if( fuel > max_fuel ) fuel = max_fuel;
}
void Satellite::ChangeLife( float mod )
{
    life += mod;
    if( life < 0 ) life = 0;
    if( life > max_life ) life = max_life;
}

void Satellite::IncrBoost()
{
    L_( "Boost doubled!\n" );
    boost *= 2;
}

void Satellite::Update( float dt )
{
    if( acc.Magnitude() > 0.5 && move_snd.GetStatus() != sf::Sound::Playing ) {
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

void Satellite::Draw( Vec2i offset )
{
    main.SetPosition( pos + offset );
    Tree::Draw( main );

    narrative.SetPos( pos + offset );
    narrative.Draw();
}

