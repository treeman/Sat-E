#include "Satellite.hpp"

Satellite::Satellite() : arrow_home(false), has_teleport(false), has_friend(false), has_coke_hat(false)
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
    boost += TWEAKS->GetNum( "acc_incr" );
    L_( "Boost :%.1f\n", boost );
}

void Satellite::IncrSpeed( float speed )
{
    max_vel += speed;
    L_( "My god MOAR SPEED: %.1f\n", max_vel );
}

void Satellite::IncrMaxFuel( float mod )
{
    max_fuel += mod;
    fuel = max_fuel;
    L_( "Fuel boost, now %.1f\n", max_fuel );
}

void Satellite::IncrArmor( float mod )
{
    max_life += mod;
    life = max_life;
    L_( "Ah I'm healthy!: %.1f\n", max_life );
}

bool Satellite::SeesWayHome()
{
    return arrow_home;
}
void Satellite::AddonWayHome()
{
    arrow_home = true;
}

bool Satellite::CanTeleport()
{
    return has_teleport;
}
void Satellite::Teleport()
{
    has_teleport = false;
    acc = Vec2f::zero;
    vel = Vec2f::zero;
}
void Satellite::AddTeleport()
{
    has_teleport = true;
}

void Satellite::AddFriend()
{
    has_friend = true;
}

void Satellite::AddCokeHat()
{
    has_coke_hat = true;
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

