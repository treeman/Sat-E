#pragma once

#include "Tree.hpp"

class Line {
public:
    Line( std::string str, float alive, float pause_before );
    Line( const Line &line );

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
    Talk( const Talk &talk );

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

    void Kill() { t.Stop(); }

    void Draw();
private:
    Vec2i pos;

    void ParseFile( std::string file );

    sf::String r_str;

    // Better random
    Tree::ShuffleBag<Talk> talks;
    Tree::ShuffleBag<int> delays;

    Talk curr_talk;
    int curr_delay;
    Tree::Timer t;

    void Update();

};

