#include "Tree.hpp"

#include "Game.hpp"

int main()
{
    L_.Init( "log.txt" );

    try {
        // Create the game
        Tree::Game::Instance()->Init(
              800             // width
            , 600             // height
            , true            // windowed
            , "Sat-E!"        // window title
            , "settings.ini"  // settings file
            , true            // lazy drawing
        );

        // Initialize shared resources
        // TODO this gets loaded automaticall... need to change it
        //Tree::GetTweaks()->Load( "magic_numbers.lua" );
        Tree::GetButler()->LoadSprites( "sprites.lua" );

        // Push your gamestates here
        {
            boost::shared_ptr<Tree::GameState> state( new Game() );
            Tree::Game::Instance()->Push( state );
        }

        // Start the main game loop
        Tree::Game::Instance()->Start();
    }
    catch( std::exception &e )
    {
        L_ << "Fatal exception: " << e.what() << '\n';
    }
    catch( ... )
    {
        L_ << "Not even Duke could handle this exception!\n";
    }

    // Destroy the singletons
    Tree::Game::Instance()->Destroy();
    L_.Destroy();

    return EXIT_SUCCESS;
}

