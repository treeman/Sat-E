#include "Space.hpp"
#include "Graphics.hpp"

Space::Space() : generator( TWEAKS->GetNum( "space_chunk" ), TWEAKS->GetNum( "space_chunk" ) )
{
    SETTINGS->Register( "bounding_box_show", false );
    SETTINGS->Register( "chunk_count", false );
    SETTINGS->Register( "show_pos", false );

    star_colors.push_back( TWEAKS->GetNum( "star_col1" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col2" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col3" ) );
    star_colors.push_back( TWEAKS->GetNum( "star_col4" ) );

    const int window_w = Tree::GetWindowWidth();
    const int window_h = Tree::GetWindowHeight();

    max_life = 100;
    max_fuel = 100;

    life = 100;
    fuel = 100;

    junk_collected = 0;

    life_spr = BUTLER->CreateSprite( "life" );
    life_spr.SetPosition( window_w - 120, window_h - 26 );

    junk_spr = BUTLER->CreateSprite( "junk" );
    junk_spr.SetPosition( 10, window_h - 30 );

    junk_str = BUTLER->CreateString( "fnt/consola.ttf", 15 );
    junk_str.SetColor( Tree::Color( 0xffcccccc ) );
    junk_str.SetPosition( 36, window_h - 28 );

    junk_snd.Add( BUTLER->CreateSound( "snd/Pickup_Coin.wav" ) )
    .Add( BUTLER->CreateSound( "snd/Pickup_Coin2.wav" ) )
    .Add( BUTLER->CreateSound( "snd/Pickup_Coin3.wav" ) )
    .Add( BUTLER->CreateSound( "snd/Pickup_Coin4.wav" ) );

    fuel_spr = BUTLER->CreateSprite( "fuel" );
    fuel_spr.SetPosition( window_w - 120, window_h - 46 );
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

    // Retract fuel
    fuel -= TWEAKS->GetNum( "fuel_speed" ) * dt;
    if( fuel < 0 ) fuel = 0;

    // Set size
    acc.SetMagnitude( TWEAKS->GetNum( "satellite_acc" ) );

    satellite.Accelerate( acc );

    // Check for closeness to our docking system
    const Vec2f docking = satellite.GetPos() - box.GetPos();
    if( docking.Magnitude() <= 30 ) { // We're close!!
        box.IsClose( true ); // Visible cue

        // Increase our fuel
        fuel += TWEAKS->GetNum( "refuel_speed" ) * dt;
        if( fuel > max_fuel ) fuel = max_fuel;
    }
    else {
        box.IsClose( false );
    }

    // Update chunks, allocate more if needed
    UpdateSpaceChunks();

    // The chunk we're at
    Chunks::iterator it = chunks.find( CurrentChunkIndex() );
    if( it != chunks.end() ) {
        Chunk &chunk = it->second;

        // Update our chunk and everything in it
        chunk.Update( dt );

        // Check intersections
        Items items = chunk.GetItems();
        for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
            if( (*it)->BoundingBox().Intersects( satellite.BoundingBox() ) ) {
                Intersects( *it );
            }
        }
    }
    else {
        L_ << "couldn't find " << CurrentChunkIndex() << " chunk for updating\n";
    }

    // Center cam on satellite
    CenterCam( satellite.GetPos() );

    // Update GUI
    junk_str.SetText( boost::lexical_cast<std::string>( junk_collected ) );
}

void Space::Draw()
{
    const Vec2i offset = -cam;
    Tree::ClearWindow( Tree::Color( 0xFF000000 ) );

    // Draw the chunk we're at
    Chunks::iterator it = chunks.find( CurrentChunkIndex() );
    if( it != chunks.end() ) {
        Chunk &chunk = it->second;
        chunk.Draw( offset );
    }
    else {
        L_ << "couldn't find " << CurrentChunkIndex() << " chunk for rendering\n";
    }

    box.Draw( offset );

    // Draw satellite
    satellite.Draw( offset );

    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        draw_outline( satellite.BoundingBox(), -cam );
    }

    // Draw GUI elements
    DrawLife();
    DrawJunk();
    DrawFuel();
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
        AllocateChunk( chunk_index );
    }

    if( SETTINGS->GetValue<bool>( "chunk_count" ) ) {
        std::stringstream ss;
        ss << chunks.size() << " chunks\n";
        Tree::VisualDebug( ss.str() );
    }
}

void Space::AllocateChunk( Vec2i chunk_index )
{
    // Make a chunk!
    Chunk chunk( ChunkRect( chunk_index ), generator );

    // Insert it
    chunks.insert( std::make_pair( chunk_index, chunk ) );
    existing_chunks.insert( chunk_index );
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

void Space::Intersects( ItemPtr item )
{
    switch( item->Behavior() ) {
        case Nothing:
            L_("I'm a master of nothing!\n");
            break;
        case AddJunk:
            JunkAdded( 1 );
            break;
        case Hurts:
            L_("Ouch!\n");
            break;
    }

    item->Kill();
}

void Space::DrawLife()
{
    const int window_w = Tree::GetWindowWidth();
    const int window_h = Tree::GetWindowHeight();

    const int w = 84;
    const int h = 10;

    const float perc = (float)life / (float)max_life;

    const int hspace = 10;
    const int wspace = 10;

    draw_bar( sf::IntRect(
        window_w - w - wspace, window_h - h - hspace,
        window_w - wspace, window_h - hspace ),
        perc,
        TWEAKS->GetNum( "life_col" ),          // foreground
        TWEAKS->GetNum( "life_background" ),   // background
        TWEAKS->GetNum( "life_edge" )          // outline
    );

    Tree::Draw( life_spr );
}

void Space::JunkAdded( int num )
{
    junk_collected += num;
    curr_junk_snd = junk_snd.Get();
    curr_junk_snd.Play();
}

void Space::DrawJunk()
{
    Tree::Draw( junk_spr );
    Tree::Draw( junk_str );
}

void Space::DrawFuel()
{
    const int window_w = Tree::GetWindowWidth();
    const int window_h = Tree::GetWindowHeight();

    const int w = 84;
    const int h = 10;

    const float perc = fuel / max_fuel;

    const int hspace = 30;
    const int wspace = 10;

    draw_bar( sf::IntRect(
        window_w - w - wspace, window_h - h - hspace,
        window_w - wspace, window_h - hspace ),
        perc,
        TWEAKS->GetNum( "fuel_col" ),          // foreground
        TWEAKS->GetNum( "fuel_background" ),   // background
        TWEAKS->GetNum( "fuel_edge" )          // outline
    );

    Tree::Draw( fuel_spr );
}

