#pragma once

#include "Tree.hpp"

class Line {
public:
    Line( std::string str, float alive, float pause_before );

    void Start();

    bool IsDone();

    const std::string Str();
private:
    std::string str;
    Tree::Timer t;
    float alive_time, pause_before;
};

class Talk {
public:
    Talk();

    void Start();

    bool IsDone();
    void Push( std::string, float alive, float pause_before );

    std::string Get();
private:
    typedef std::vector<Line> Lines;
    Lines lines;

    void Update();
};

class Narrative {
public:
    Narrative();

    void SetPos( Vec2i pos );

    void Draw();
private:
    Vec2i pos;

    void ParseFile( std::string file );

    sf::String r_str;

    Talk talk;
};

