#pragma once

#include <boost/shared_ptr.hpp>

#include "Satellite.hpp"
#include "Powerup.hpp"
#include "Box.hpp"

struct Star {
    Vec2i pos;
    float power;
    Tree::Color color;
};

class Space {
public:
    Space();

    void Update( float dt );
    void Draw();
private:
    Satellite satellite;

    typedef std::vector<Star> Stars;
    Stars stars;

    sf::Sprite star_spr;

    typedef std::vector<boost::shared_ptr<Pickup> > Pickups;
    Pickups pickups;

    void DrawOutline( sf::IntRect box );

    Box box;
};

