#pragma once

#include "Tree.hpp"
#include "Satellite.hpp"

enum Action {
    Exit,
    Battery,
    Acceleration,
    Speed,
};

struct Selection {
    Selection() : available(true) { }

    bool available;
    std::string txt;
    Action action;
};

class Dock {
public:
    Dock( Satellite &satellite );

    bool IsActive();
    void Activate();
    void Deactivate();

    bool HandleEvent( sf::Event &e );

    void Update( float dt );
    void Draw();
private:
    bool is_active;

    sf::String str;

    void AddSelection( std::string txt, Action action = Exit );
    std::vector<Selection> selections;

    int curr_selection;

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void Execute();

    sf::Sound bad_snd;
    sf::Sound select_snd;
    sf::Sound move_snd;

    Satellite &satellite;
};

