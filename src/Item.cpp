#include "Item.hpp"

Item::Item() : can_kill(false)
{

}

sf::IntRect Item::BoundingBox()
{
    return sf::IntRect();
}

void Item::Accelerate( Vec2f _acc )
{
    acc += _acc;
}

bool Item::CanKill()
{
    return can_kill;
}

void Item::Kill()
{
    can_kill = true;
}

void Item::Update( float dt )
{
    UpdateMovement( dt );
}
void Item::UpdateMovement( float dt )
{
    const float max_vel = MaxVel();

    vel += acc * dt;
    if( vel.Magnitude() > max_vel ) {
        vel.SetMagnitude( max_vel );
    }

    pos += vel * dt;

    acc = Vec2f::zero;
}

