#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "Game.hpp"

Game::Game()
{
}

bool Game::HandleEvent( sf::Event &e )
{
    space.HandleEvent( e );
    return true;
}

void Game::Update( float dt )
{
    space.Update( dt );
}

void Game::Draw()
{
    space.Draw();
}

