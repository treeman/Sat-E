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

    void Update( float dt );
    void Draw( Vec2i offset );
private:
    sf::Sprite main;

    sf::Sound move_snd;

    Narrative narrative;

    float max_vel;
    float max_fuel;
    float max_life;

    float life;
    float fuel;

    float boost;
};

