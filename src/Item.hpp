#pragma once

#include "Tree.hpp"

class Item;

// Lazy zzz
typedef boost::shared_ptr<Item> ItemPtr;
typedef std::vector<ItemPtr> Items;

enum ItemBehavior {
    Nothing,
    AddJunk,
    Hurts,
};

class Item {
public:
    Item();

    virtual ~Item() { }

    virtual sf::IntRect BoundingBox();

    void Accelerate( Vec2f acc );

    // Set speed
    virtual float MaxVel() { return 0; }

    const Vec2f GetPos() { return pos; }
    const Vec2f GetVel() { return vel; }
    const Vec2f GetAcc() { return acc; }

    void SetPos( Vec2f p ) { pos = p; }

    bool CanKill();
    void Kill();

    // Behavior when intersecting with it
    virtual ItemBehavior Behavior() { return Nothing; }

    virtual void Update( float dt );
    virtual void Draw( Vec2i offset ) = 0;
protected:
    void UpdateMovement( float dt );

    Vec2f pos;
    Vec2f vel;
    Vec2f acc;

    bool can_kill;
};

