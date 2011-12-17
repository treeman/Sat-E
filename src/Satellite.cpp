#include "Satellite.hpp"

Satellite::Satellite() : pos( 100, 100 ), vel( 0, 0 ), acc( 0, 0 )
{
    main = BUTLER->CreateSprite( "gfx/basic.png" );
    move_snd = BUTLER->CreateSound( "snd/burst1.wav" );
    move_snd.SetLoop( true );
}

sf::IntRect Satellite::BoundingBox()
{
    return sf::IntRect( pos.x, pos.y, pos.x + 20, pos.y + 20 );
}

void Satellite::Accelerate( Vec2f _acc )
{
    acc = _acc;
}

void Satellite::Update( float dt )
{
    const float max_vel = TWEAKS->GetNum( "satellite_max_vel" );
    const float acc_size = TWEAKS->GetNum( "satellite_acc" );

    if( acc != Vec2f::zero && move_snd.GetStatus() != sf::Sound::Playing ) {
        move_snd.Play();
    }
    else if( acc == Vec2f::zero ) {
        move_snd.Stop();
    }

    acc.SetMagnitude( acc_size );

    vel += acc * dt;
    if( vel.Magnitude() > max_vel ) {
        vel.SetMagnitude( max_vel );
    }

    pos += vel * dt;

    main.SetPosition( pos );

    Vec2f comp( 0, 1 );
    float angle = atan2( vel.y, vel.x ) - atan2( comp.y, comp.x ) + math::PI;

    float degree = angle * 180 / math::PI;
    main.SetRotation( degree );

    std::stringstream ss;
    ss << vel << " => " << degree;
    Tree::VisualDebug( "angle", ss.str() );

    acc = Vec2f::zero;
}

void Satellite::Draw()
{
    Tree::Draw( main );
}

