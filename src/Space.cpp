#include "Space.hpp"

Space::Space()
{
    SETTINGS->Register( "bounding_box_show", false );

    // Init stars
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

    // Initialize stuff in space w00p

    // 'Random' positions
    for( int x = 0; x < 100; ++x ) {
        for( int y = 0; y < 100; ++y ) {
            position_bag.Add( Vec2i( x * 10, y * 10 ) );
        }
    }

    // Setup junk sprites
    InitJunk();

    // Add in some junk on screen
    for( int i = 0; i < 10; ++i ) {
        boost::shared_ptr<Item> item( new Junk( junk_bag.Get() ) );
        item->SetPos( position_bag.Get() );
        items.push_back( item );
    }
}

void Space::Update( float dt )
{
    box.Update( dt );

    satellite.Update( dt );

    // Update movements for the satellite in a fluid manner
    const sf::Input &input = GAME->GetInput();

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

    // Check for closeness to our docking system
    const Vec2f docking = satellite.GetPos() - box.GetPos();
    if( docking.Magnitude() <= 30 ) {
        box.IsClose( true );
    }
    else {
        box.IsClose( false );
    }

    for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
        (*it)->Update( dt );
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
        col.a = 0xFF * ( stars[i].power + math::frandom( 0, 0.1 ) );
        star_spr.SetColor( col );

        Tree::Draw( star_spr );
    }

    // Draw pickups
    for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
        (*it)->Draw();
    }

    box.Draw();

    // Draw satellite
    satellite.Draw();

    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        DrawOutline( satellite.BoundingBox() );

        // Draw outlines for pickups
        for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
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

void Space::InitJunk()
{
    // Get a list of junk
    Tree::ImgPtr img = BUTLER->GetImage( "gfx/junk.png" );

    const int width = 20;
    const int height = 20;

    const int num_w = img->GetWidth() / width;
    const int num_h = img->GetHeight() / height;
    const int images = num_w * num_h;

    //L_( "%dx%d = %d\n", img->GetWidth(), img->GetHeight(), images );

    for( int i = 0; i < images; ++i ) {
        const int x_index = i % num_w;
        const int y_index = i / num_w;

        const int x = x_index * width;
        const int y = y_index * height;

        sf::Sprite spr;
        spr.SetImage( *img );
        spr.SetSubRect( sf::IntRect( x, y, x + width, y + height ) );
        spr.SetCenter( width / 2, height / 2 );

        junk_bag.Add( spr );
    }
}

