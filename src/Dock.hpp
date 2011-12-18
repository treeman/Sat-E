#pragma once

#include "Tree.hpp"
#include "Satellite.hpp"

enum Action {
    Battery,
    Acceleration,
    Speed,
    Armor,
    Exchange,
    Reciever,
    Teleport,
    CokeHat,
    Friend,
    Exit,

    NumActions,
};

struct Selection {
    Selection() : available(true), count(0) { }

    bool available;
    Action action;
    int count;
    std::string txt;
};

class Dock {
public:
    Dock( Satellite &satellite );

    bool IsActive();
    void Activate();
    void Deactivate();

    bool HandleEvent( sf::Event &e );

    void AddTeleport();

    void Update( float dt );
    void Draw();
private:
    bool is_active;

    sf::String str;

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

