#include "Satellite.hpp"

Satellite::Satellite() : arrow_home(false), has_teleport(false), has_friend(false), has_beer_cap(false),
    speed_lvl(0), has_parabol(false)
{
    pos = Vec2f( 100, 100 );

    // Sprite loading
    main_spr = BUTLER->CreateSprite( "base_satellite" );
    main_spr.SetCenter( 7, 7 );

    beer_cap_spr = BUTLER->CreateSprite( "beer_cap" );
    beer_cap_spr.SetCenter( 7, 7 );

    speed1_spr = BUTLER->CreateSprite( "speed1" );
    speed1_spr.SetCenter( 7, 7 );

    speed2_spr = BUTLER->CreateSprite( "speed2" );
    speed2_spr.SetCenter( 7, 7 );

    teleport_spr = BUTLER->CreateSprite( "teleport" );
    teleport_spr.SetCenter( 7, 7 );

    recievers_spr = BUTLER->CreateSprite( "recievers" );
    recievers_spr.SetCenter( 14, 7 );

    parabol_spr = BUTLER->CreateSprite( "parabol" );
    parabol_spr.SetCenter( 7, 7 );

    // Other stuff ^^
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
    has_parabol = true;
}

void Satellite::IncrSpeed( float speed )
{
    max_vel += speed;
    L_( "My god MOAR SPEED: %.1f\n", max_vel );
    ++speed_lvl;
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

void Satellite::AddBeerCap()
{
    has_beer_cap = true;
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

    main_spr.Rotate( degree * dt );
}

void Satellite::Draw( Vec2i offset )
{
    Vec2i draw_pos = pos + offset;
    float rot = main_spr.GetRotation();

    main_spr.SetPosition( draw_pos );
    Tree::Draw( main_spr );

    if( has_parabol ) {
        parabol_spr.SetPosition( draw_pos );
        parabol_spr.SetRotation( rot );
        Tree::Draw( parabol_spr );
    }

    if( speed_lvl == 1 ) {
        speed1_spr.SetPosition( draw_pos );
        speed1_spr.SetRotation( rot );
        Tree::Draw( speed1_spr );
    }
    else if( speed_lvl > 1 ) {
        speed2_spr.SetPosition( draw_pos );
        speed2_spr.SetRotation( rot );
        Tree::Draw( speed2_spr );
    }

    if( has_beer_cap ) {
        beer_cap_spr.SetPosition( draw_pos );
        beer_cap_spr.SetRotation( rot );
        Tree::Draw( beer_cap_spr );
    }

    if( has_teleport ) {
        teleport_spr.SetPosition( draw_pos );
        teleport_spr.SetRotation( rot );
        Tree::Draw( teleport_spr );
    }

    if( arrow_home ) {
        recievers_spr.SetPosition( draw_pos );
        recievers_spr.SetRotation( rot );
        Tree::Draw( recievers_spr );
    }

    narrative.SetPos( draw_pos );
    narrative.Draw();
}

