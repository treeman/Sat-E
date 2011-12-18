#include "Space.hpp"
#include "Graphics.hpp"

Space::Space() : generator( TWEAKS->GetNum( "space_chunk" ), TWEAKS->GetNum( "space_chunk" ) ), dock( satellite )
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

    can_activate_docking = false;

    satellite.SetPos( 480, 480 );
    box.SetPos( 500, 500 );

    arrow_home_spr = BUTLER->CreateSprite( "arrowhome" );

    hit_snd = BUTLER->CreateSound( "snd/Exp.wav" );
    heal_snd = BUTLER->CreateSound( "snd/heal.wav" );
}

void Space::HandleEvent( sf::Event &e )
{
    if( dock.IsActive() ) {
        dock.HandleEvent( e );
    }
    else {
        if( e.Type == sf::Event::KeyPressed ) {
            switch( e.Key.Code ) {
                case sf::Key::Space:
                    if( satellite.CanTeleport() ) {
                        Teleport();
                    }
                    break;
                default: break;
            }
        }
    }
}

void Space::Update( float dt )
{
    if( dock.IsActive() ) {
        dock.Update( dt );
    }
    else {
        satellite.Update( dt ); // prevent movement when in docking mode

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
        acc.SetMagnitude( satellite.AccBoost() );

        satellite.Accelerate( acc );
    }

    box.Update( dt );

    // Retract fuel
    satellite.ChangeFuel( - TWEAKS->GetNum( "fuel_speed" ) * dt );

    // Off centered... Would want a better way but meh
    Vec2f sat_pos = satellite.GetPos();
    sat_pos.x += 5; sat_pos.y += 5;

    const Vec2f docking = sat_pos - box.GetPos();

    // Check for closeness to our docking system
    if( docking.Magnitude() <= 30 ) { // We're close!!
        box.IsClose( true ); // Visible cue

        // Increase our fuel
        satellite.ChangeFuel( TWEAKS->GetNum( "refuel_speed" ) * dt );

        // If we just entered docking
        if( docking.Magnitude() <= 25 ) {
            if( can_activate_docking ) {
                dock.Activate();
                can_activate_docking = false;
            }
        }
    }
    else {
        box.IsClose( false );
        can_activate_docking = true;
    }

    // Home arrow
    if( satellite.SeesWayHome() && docking.Magnitude() > 200 ) {
        Vec2f show_home = docking;
        show_home.SetMagnitude( 50 );
        Vec2f arrow_pos = sat_pos - show_home;
        arrow_home_spr.SetPosition( arrow_pos );

        float angle = - atan2( show_home.y, show_home.x );

        float degree = 180 / math::PI * angle;
        arrow_home_spr.SetRotation( degree );
    }

    // Update chunks, allocate more if needed
    UpdateChunks( dt );

    // Center cam on satellite
    CenterCam( satellite.GetPos() );

    // Update GUI
    junk_str.SetText( boost::lexical_cast<std::string>( junk_collected ) );
}

void Space::Draw()
{
    Vec2i offset = -cam;
    Tree::ClearWindow( Tree::Color( 0xFF000000 ) );

    Vec2i chunk = CurrentChunkIndex();

    DrawChunk( chunk, offset );

    // Draw surrounding chunks
    DrawChunk( chunk.x - 1, chunk.y, offset );
    DrawChunk( chunk.x + 1, chunk.y, offset );
    DrawChunk( chunk.x, chunk.y - 1, offset );
    DrawChunk( chunk.x, chunk.y + 1, offset );

    DrawChunk( chunk.x - 1, chunk.y - 1, offset );
    DrawChunk( chunk.x - 1, chunk.y + 1, offset );
    DrawChunk( chunk.x + 1, chunk.y - 1, offset );
    DrawChunk( chunk.x + 1, chunk.y + 1, offset );

    box.Draw( offset );

    // Draw satellite
    satellite.Draw( offset );

    if( satellite.SeesWayHome() ) {
        arrow_home_spr.Move( offset );
        Tree::Draw( arrow_home_spr );
    }

    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        draw_outline( satellite.BoundingBox(), -cam );
    }

    // Draw GUI elements
    DrawLife();
    DrawJunk();
    DrawFuel();

    // Draw docking on top, if we can ofc
    if( dock.IsActive() ) { dock.Draw(); }
}

void Space::DrawChunk( int x, int y, Vec2i offset ) { DrawChunk( Vec2i( x, y), offset ); }
void Space::DrawChunk( Vec2i index, Vec2i offset )
{
    Chunks::iterator it = chunks.find( index );
    if( it != chunks.end() ) {
        Chunk &chunk = it->second;
        chunk.Draw( offset, sf::IntRect( cam.x, cam.y, cam.x + Tree::GetWindowWidth(), cam.y + Tree::GetWindowHeight() ) );
    }
    else {
        L_( "Couldn't find %d chunk for rendering" );
    }
}

void Space::UpdateChunks( float dt )
{
    const Vec2i chunk_index = CurrentChunkIndex();

    if( SETTINGS->GetValue<bool>( "show_pos" ) ) {
        std::stringstream ss;
        Vec2i ip = satellite.GetPos();
        ss << "pos: " << ip << " in chunk " << chunk_index << '\n';
        Tree::VisualDebug( ss.str() );
    }

    // Not yet checked chunk
    if( checked_chunks.find( chunk_index ) == checked_chunks.end() ) {
        // Allocate this chunk (probably done but it gets checked anyway
        AllocateChunk( chunk_index );
        // Allocate surrounding chunks
        AllocateChunk( chunk_index.x - 1, chunk_index.y );
        AllocateChunk( chunk_index.x + 1, chunk_index.y );
        AllocateChunk( chunk_index.x, chunk_index.y - 1 );
        AllocateChunk( chunk_index.x, chunk_index.y + 1 );

        AllocateChunk( chunk_index.x - 1, chunk_index.y - 1 );
        AllocateChunk( chunk_index.x - 1, chunk_index.y + 1 );
        AllocateChunk( chunk_index.x + 1, chunk_index.y - 1 );
        AllocateChunk( chunk_index.x + 1, chunk_index.y + 1 );
    }

    // Debug current chunks
    if( SETTINGS->GetValue<bool>( "chunk_count" ) ) {
        std::stringstream ss;
        ss << chunks.size() << " chunks\n";
        Tree::VisualDebug( ss.str() );
    }

    // Update all visible chunks!
    UpdateChunk( chunk_index, dt );

    UpdateChunk( chunk_index.x - 1, chunk_index.y, dt );
    UpdateChunk( chunk_index.x + 1, chunk_index.y, dt );
    UpdateChunk( chunk_index.x, chunk_index.y - 1, dt );
    UpdateChunk( chunk_index.x, chunk_index.y + 1, dt );

    UpdateChunk( chunk_index.x - 1, chunk_index.y - 1, dt );
    UpdateChunk( chunk_index.x - 1, chunk_index.y + 1, dt );
    UpdateChunk( chunk_index.x + 1, chunk_index.y - 1, dt );
    UpdateChunk( chunk_index.x + 1, chunk_index.y + 1, dt );
}

void Space::UpdateChunk( int x, int y, float dt )
{
    UpdateChunk( Vec2i( x, y ), dt );
}
void Space::UpdateChunk( Vec2i index, float dt )
{
    // The chunk we're at
    Chunks::iterator it = chunks.find( index );
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
        L_("couldn't find %d index chunk for updating\n");
    }
}

void Space::AllocateChunk( int x, int y ) { AllocateChunk( Vec2i( x, y ) ); }
void Space::AllocateChunk( Vec2i chunk_index )
{
    // Already allocated
    if( existing_chunks.find( chunk_index ) != existing_chunks.end() ) return;

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
            satellite.ChangeLife( -TWEAKS->GetNum( "asteroid_damage" ) );
            hit_snd.Play();
            break;
        case Heal:
            satellite.ChangeLife( TWEAKS->GetNum( "asteroid_damage" ) );
            heal_snd.Play();
            break;
        default: break;
    }

    item->Kill();
}

void Space::DrawLife()
{
    const int window_w = Tree::GetWindowWidth();
    const int window_h = Tree::GetWindowHeight();

    const int w = 84;
    const int h = 10;

    const float perc = satellite.Life() / satellite.MaxLife();

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

    const float perc = satellite.Fuel() / satellite.MaxFuel();

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

void Space::Teleport()
{
    satellite.Teleport();
    dock.AddTeleport();
    satellite.SetPos( 480, 480 );

    can_activate_docking = false;
}

