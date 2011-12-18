#pragma once

#include "Item.hpp"

class SimpleItem : public Item {
public:
    SimpleItem( sf::Sprite _spr, ItemBehavior _b, float rot = 0 ) : spr(_spr), behavior(_b), rotation(rot)
    { }

    sf::IntRect BoundingBox() {
        return sf::IntRect( pos.x -5, pos.y -5, pos.x + 5, pos.y + 5 );
    }

    ItemBehavior Behavior() { return behavior; }

    void Update( float dt ) {
        UpdateMovement( dt );
        spr.Rotate( rotation * dt );
    }

    void Draw( Vec2i offset ) {
        spr.SetPosition( pos + offset );
        Tree::Draw( spr );
    }
protected:
    sf::Sprite spr;
    ItemBehavior behavior;
    float rotation;
};

class Junk : public SimpleItem {
public:
    Junk( sf::Sprite spr ) : SimpleItem( spr, AddJunk, TWEAKS->GetNum( "junk_rotate" ) ) { }
};

class Asteroid : public SimpleItem {
public:
    Asteroid( sf::Sprite spr ) : SimpleItem( spr, Hurts )
    {
        const float max = 10;
        vel = Vec2i( math::frandom( -max, max ), math::frandom( -max, max ) );
    }

    float MaxVel() { return 100; }

    void Update( float dt ) {
        UpdateMovement( dt );
        spr.Rotate( math::frandom( 0.5, 1.0 ) * dt );
    }
};

class Healer : public SimpleItem {
public:
    Healer( sf::Sprite spr ) : SimpleItem( spr, Heal, TWEAKS->GetNum( "junk_rotate" ) ) { }
};

class Coveted : public SimpleItem {
public:
    Coveted( sf::Sprite spr ) : SimpleItem( spr, AddCovet, TWEAKS->GetNum( "junk_rotate" ) ) { }
};

