#include "Space.hpp"

Space::Space()
{
    SETTINGS->Register( "bounding_box_show", false );
    SETTINGS->Register( "chunk_count", false );
    SETTINGS->Register( "show_pos", false );

    star_colors.push_back( TWEAKS->GetNum( "star_col1" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col2" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col3" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col4" ) );

    // 'Random' positions
    for( int x = 0; x < 100; ++x ) {
        for( int y = 0; y < 100; ++y ) {
            position_bag.Add( Vec2i( x * 10, y * 10 ) );
        }
    }

    // Setup junk sprites
    InitJunk();

    // Add in stuff to our window screen
    //AllocateChunk( sf::IntRect( 0, 0, Tree::GetWindowWidth(), Tree::GetWindowHeight() ) );
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

    //for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
        //(*it)->Update( dt );
        //if( (*it)->BoundingBox().Intersects( satellite.BoundingBox() ) ) {
            //L_ << "Intersecting!\n";
        //}
    //}

    // Update chunks, allocate more if needed
    UpdateSpaceChunks();

    // Center cam on satellite
    CenterCam( satellite.GetPos() );
}

void Space::Draw()
{
    const Vec2i offset = -cam;
    Tree::ClearWindow( Tree::Color( 0xFF000000 ) );

    Chunks::iterator it = chunks.find( CurrentChunkIndex() );
    if( it != chunks.end() ) {
        Chunk &chunk = it->second;
        chunk.Draw( offset );
    }
    else {
        L_ << "Couldn't find " << CurrentChunkIndex() << " chunk for rendering\n";
    }

    //const sf::IntRect visible_chunk( cam.x, cam.y, Tree::GetWindowWidth(), Tree::GetWindowHeight() );

    //// Draw stars
    //for( size_t i = 0; i < stars.size(); ++i ) {
        //Vec2i pos = stars[i].pos + offset;

        //if( !visible_chunk.Contains( pos.x, pos.y ) ) continue;

        //star_spr.SetPosition( pos );

        //Tree::Color col = stars[i].color;
        //col.a = 0xFF * ( stars[i].power + math::frandom( 0, 0.1 ) );
        //star_spr.SetColor( col );

        //Tree::Draw( star_spr );
    //}

    // Draw items
    for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
        (*it)->Draw( offset );
    }

    box.Draw( offset );

    // Draw satellite
    satellite.Draw( offset );

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

    border.Move( -cam );

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

void Space::UpdateSpaceChunks()
{
    const Vec2i chunk_index = CurrentChunkIndex();

    if( SETTINGS->GetValue<bool>( "show_pos" ) ) {
        std::stringstream ss;
        Vec2i ip = satellite.GetPos();
        ss << "pos: " << ip << " in chunk " << chunk_index << '\n';
        Tree::VisualDebug( ss.str() );
    }

    // Not yet allocated
    if( existing_chunks.find( chunk_index ) == existing_chunks.end() ) {
        // Make a chunk!
        Chunk chunk( ChunkRect( chunk_index ) );
        // Insert it
        chunks.insert( std::make_pair( chunk_index, chunk ) );
        existing_chunks.insert( chunk_index );
    }

    if( SETTINGS->GetValue<bool>( "chunk_count" ) ) {
        std::stringstream ss;
        ss << chunks.size() << " chunks\n";
        Tree::VisualDebug( ss.str() );
    }
}

// Where are we?
Vec2i Space::CurrentChunkIndex()
{
    const Vec2i center = satellite.GetPos();
    const int chunk_size = TWEAKS->GetNum( "space_chunk" );
    Vec2i chunk( center.x / chunk_size, center.y / chunk_size );

    // One off if we're going the 'wrong' direction
    if( center.x < 0 ) chunk -= Vec2i( 1, 0 );
    if( center.y < 0 ) chunk -= Vec2i( 0, 1 );

    return chunk;
}

sf::IntRect Space::ChunkRect( Vec2i chunk )
{
    const int chunk_size = TWEAKS->GetNum( "space_chunk" );
    const Vec2i topleft = chunk * 1000;
    return sf::IntRect( topleft.x, topleft.y, topleft.x + chunk_size, topleft.y + chunk_size );
}

void Space::CenterCam( Vec2i pos )
{
    const int w = Tree::GetWindowWidth();
    const int h = Tree::GetWindowHeight();

    cam.x = pos.x - w / 2;
    cam.y = pos.y - h / 2;
}

