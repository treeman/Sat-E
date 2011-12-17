#pragma once

#include "Tree.hpp"

class Line {
public:
    Line( std::string str, float alive );

    bool IsDone();

    void DrawAt( Vec2i pos );
private:
    std::string str;
    Tree::Timer t;
    float alive_time;

    sf::String r_str;
};

class Narrative {
public:
    Narrative();

    void SetPos( Vec2f pos );

    void Draw();
private:
    Vec2f pos;
};

