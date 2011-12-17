#include "Space.hpp"

Space::Space()
{
    star_spr = BUTLER->CreateSprite( "gfx/star.png" );

    std::vector<double> colors;
    colors.push_back( TWEAKS->GetNum( "star_col1" ) );
    colors.push_back( TWEAKS->GetNum( "star_col2" ) );
    colors.push_back( TWEAKS->GetNum( "star_col3" ) );
    colors.push_back( TWEAKS->GetNum( "star_col4" ) );

    for( size_t i = 0; i < 100; ++i ) {
        Star star;
        star.pos = Vec2i( math::irandom( 0, 800 ), math::irandom( 0, 600 ) );
        star.power = math::frandom( 0, 0.4 );

        star.color = Tree::Color( *math::random( colors.begin(), colors.end() ) );

        stars.push_back( star );
    }

    boost::shared_ptr<Powerup> powerup( new Powerup() );
    powerup->SetPos( Vec2f( 200, 300 ) );
    pickups.push_back( powerup );

    SETTINGS->Register( "bounding_box_show", false );
}

void Space::Update( float dt )
{
    satellite.Update( dt );

    const sf::Input &input = GAME->GetInput();

    // Update movements for the satellite in a fluid manner
    // Should move to controller or something
    //
    Vec2i acc;
    // Set direction
    if( input.IsKeyDown( sf::Key::Left ) ) {
        acc += Vec2f::left;
    }
    if( input.IsKeyDown( sf::Key::Right ) ) {
        acc += Vec2f::right;
    }
    if( input.IsKeyDown( sf::Key::Up ) ) {
        acc += Vec2f::up;
    }
    if( input.IsKeyDown( sf::Key::Down ) ) {
        acc += Vec2f::down;
    }

    // Set size
    acc.SetMagnitude( TWEAKS->GetNum( "satellite_acc" ) );

    satellite.Accelerate( acc );

    for( Pickups::iterator it = pickups.begin(); it != pickups.end(); ++it ) {
        if( (*it)->BoundingBox().Intersects( satellite.BoundingBox() ) ) {
            L_ << "Intersecting!\n";
        }
    }
}

void Space::Draw()
{
    Tree::ClearWindow( Tree::Color( 0xFF000000 ) );

    // Draw stars
    for( size_t i = 0; i < stars.size(); ++i ) {
        star_spr.SetPosition( stars[i].pos );

        Tree::Color col = stars[i].color;
        col.a = 0xFF * stars[i].power;
        star_spr.SetColor( col );

        Tree::Draw( star_spr );
    }

    // Draw pickups
    for( Pickups::iterator it = pickups.begin(); it != pickups.end(); ++it ) {
        (*it)->Draw();
    }


    // Draw satellite
    satellite.Draw();

    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        DrawOutline( satellite.BoundingBox() );

        // Draw outlines for pickups
        for( Pickups::iterator it = pickups.begin(); it != pickups.end(); ++it ) {
            DrawOutline( (*it)->BoundingBox() );
        }
    }
}

void Space::DrawOutline( sf::IntRect box )
{
    sf::Shape border = sf::Shape::Rectangle( box.Left, box.Top, box.Right, box.Bottom,
        Tree::Color( 0xFFFFFFFF ), 1.0, Tree::Color( 0xFFFFFFFF ) );
    border.EnableFill( false );
    border.EnableOutline( true );

    Tree::Draw( border );
}

