#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "Game.hpp"

Game::Game()
{
    Tree::GetTweaks()->Load( "magic_numbers.lua" );
}

bool Game::HandleEvent( sf::Event &e )
{

    return true;
}

void Game::Update( float dt )
{
    const sf::Input &input = GAME->GetInput();

    // Update movements for the satellite in a fluid manner
    if( input.IsKeyDown( sf::Key::Left ) ) {
        satellite.Accelerate( Vec2f::left );
    }
    if( input.IsKeyDown( sf::Key::Right ) ) {
        satellite.Accelerate( Vec2f::right );
    }
    if( input.IsKeyDown( sf::Key::Up ) ) {
        satellite.Accelerate( Vec2f::up );
    }
    if( input.IsKeyDown( sf::Key::Down ) ) {
        satellite.Accelerate( Vec2f::down );
    }

    satellite.Update( dt );
}

void Game::Draw()
{
    Tree::ClearWindow( Tree::Color( 0xFF000000 ) );

    satellite.Draw();
}

