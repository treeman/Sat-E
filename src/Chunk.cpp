#include "Chunk.hpp"
#include "Graphics.hpp"

bool can_kill( ItemPtr item )
{
    return item->CanKill();
}

Chunk::Chunk( sf::IntRect rect, ItemGenerator _generator, int distance ) :
    start_pos( rect.Left, rect.Top ), generator(_generator)
{
    //L_( "making chunk for %d,%d - %d,%d\n", rect.Left, rect.Top, rect.Right, rect.Bottom );
    //

    const int max_distance = TWEAKS->GetNum( "outer_rim" );
    float gen = (float)distance / (float)max_distance;
    if( distance > max_distance ) gen = 1;

    rand = math::frandom( 0.5, 1.5 ) * gen;

    size_t num_junk;
    size_t num_asteroids;
    size_t num_healers;
    size_t num_coveted;

    // extremely rich area
    if( rand > 1.0 ) {
        num_junk = math::irandom( 100, 200 );
        num_asteroids = math::irandom( 40, 100 );
        num_healers = math::irandom( 10, 40 );
        num_coveted = math::irandom( 20, 40 );
    }
    // high
    else if( rand > 0.8 ) {
        num_junk = math::irandom( 50, 100 );
        num_asteroids = math::irandom( 10, 40 );
        num_healers = math::irandom( 5, 20 );
        num_coveted = math::irandom( 10, 20 );
    }
    // medium
    else if( rand > 0.4 ) {
        num_junk = math::irandom( 30, 50 );
        num_asteroids = math::irandom( 10, 30 );
        num_healers = math::irandom( 2, 10 );
        num_coveted = math::irandom( 3, 10 );
    }
    // moderately low
    else if( rand > 0.2 ) {
        num_junk = math::irandom( 15, 20 );
        num_asteroids = math::irandom( 5, 10 );
        num_healers = math::irandom( 0, 5 );
        num_coveted = math::irandom( 0, 2 );
    }
    // low
    else if( rand > 0.1 ) {
        num_junk = math::irandom( 10, 15 );
        num_asteroids = math::irandom( 4, 8 );
        num_healers = math::irandom( 0, 3 );
        num_coveted = math::irandom( 0, 1 );
    }
    else if( rand > 0.01 ) {
        num_junk = math::irandom( 6, 10 );
        num_asteroids = math::irandom( 2, 6 );
        num_healers = math::irandom( 0, 3 );
        num_coveted = 0;
    }
    // very sparse
    else {
        num_junk = math::irandom( 4, 6 );
        num_asteroids = math::irandom( 1, 2 );
        num_healers = math::irandom( 0, 1 );
        num_coveted = 0;
    }

    //L_ << "d: " << distance << " max: " << max_distance << " gen: " << gen << " rand: " << rand << '\n';
    //L_("%d - %f\n", distance, rand);

    // Fill up with stars
    float area = std::abs( rect.Left - rect.Right ) * std::abs( rect.Top - rect.Bottom );

    //L_ << TWEAKS->GetNum( "star_density" ) << '\n';
    const size_t num_stars = area * TWEAKS->GetNum( "star_density" );

    //L_( "area %f => %d stars\n", area, num_stars );

    // Set some stars
    for( size_t i = 0; i < num_stars; ++i ) {
        stars.push_back( generator.CreateStar( start_pos) );
    }

    // Generate some junk
    for( size_t i = 0; i < num_junk; ++i ) {
        items.push_back( generator.CreateJunk( start_pos ) );
    }

    // Generate some asteroids
    for( size_t i = 0; i < num_asteroids; ++i ) {
        items.push_back( generator.CreateAsteroid( start_pos ) );
    }

    // Generate some healing stuff
    for( size_t i = 0; i < num_healers; ++i ) {
        items.push_back( generator.CreateHealer( start_pos ) );
    }

    // Generate some coveted stuff
    for( size_t i = 0; i < num_coveted; ++i ) {
        items.push_back( generator.CreateCoveted( start_pos ) );
    }
}

Items Chunk::GetItems()
{
    return items;
}

void Chunk::Update( float dt )
{
    // Remove killable items
    items.erase( std::remove_if( items.begin(), items.end(), can_kill ), items.end() );

    // Update items
    for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
        (*it)->Update( dt );
    }
}

void Chunk::Draw( Vec2i offset, sf::IntRect visible )
{
    // Draw stars
    for( size_t i = 0; i < stars.size(); ++i ) {
        const Vec2i &pos = stars[i].GetPos();
        if( visible.Contains( pos.x, pos.y ) ) {
            stars[i].Draw( offset );
        }
    }

    // Draw items
    for( size_t i = 0; i < items.size(); ++i ) {
        const Vec2i &pos = items[i]->GetPos();
        if( visible.Contains( pos.x, pos.y ) ) {
            items[i]->Draw( offset );
        }
    }

    // Draw outlines
    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
            const Vec2i &pos = (*it)->GetPos();
            if( visible.Contains( pos.x, pos.y ) ) {
                draw_outline( (*it)->BoundingBox(), offset );
            }
        }
    }
}

