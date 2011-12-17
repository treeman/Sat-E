#include "Satellite.hpp"

Satellite::Satellite() : pos( 100, 100 ), vel( 0, 0 ), acc( 0, 0 )
{
    main = BUTLER->CreateSprite( "gfx/basic.png" );
}

void Satellite::Accelerate( Vec2f _acc )
{
    acc = _acc;
    L_ << "accing " << acc << '\n';
}

void Satellite::Update( float dt )
{
    const float max_vel = TWEAKS->GetNum( "satellite_max_vel" );
    const float acc_size = TWEAKS->GetNum( "satellite_acc" );

    acc.SetMagnitude( acc_size );

    vel += acc;
    if( vel.Magnitude() > max_vel ) {
        vel.SetMagnitude( max_vel );
    }

    pos += vel * dt;

    main.SetPosition( pos );

    acc = Vec2f::zero;
}

void Satellite::Draw()
{
    Tree::Draw( main );
}

