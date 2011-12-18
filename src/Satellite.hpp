#pragma once

#include "Tree.hpp"

#include "Item.hpp"
#include "Narrative.hpp"

class Satellite : public Item {
public:
    Satellite();

    sf::IntRect BoundingBox();

    float MaxVel() { return max_vel; }
    float MaxFuel() { return max_fuel; }
    float MaxLife() { return max_life; }

    float Fuel() { return fuel; }
    float Life() { return life; }

    void ChangeFuel( float mod );
    void ChangeLife( float mod );

    float AccBoost() { return boost; }

    void IncrBoost();
    void IncrSpeed( float speed );
    void IncrMaxFuel( float mod );
    void IncrArmor( float mod ); // actually life... but w/e

    bool SeesWayHome();   // Arrow pointing home?
    void AddonWayHome(); // We can see home now?

    bool CanTeleport();
    void Teleport();
    void AddTeleport();

    void AddFriend(); // w00p!
    bool HasFriend() { return has_friend; }

    void AddBeerCap();

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sound move_snd;

    Narrative narrative;

    float max_vel;
    float max_fuel;
    float max_life;

    float life;
    float fuel;

    float boost;

    bool arrow_home;
    bool has_teleport;
    bool has_friend;

    sf::Sprite main_spr;
    sf::Sprite beer_cap_spr;

    sf::Sprite speed1_spr;
    sf::Sprite speed2_spr;

    sf::Sprite teleport_spr;
    sf::Sprite recievers_spr;
    sf::Sprite parabol_spr;

    bool has_beer_cap;
    int speed_lvl;
    bool has_parabol;
};

