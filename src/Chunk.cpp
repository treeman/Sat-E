
#include "Chunk.hpp"

Chunk::Chunk( sf::IntRect rect, ItemGenerator _generator ) :
    start_pos( rect.Left, rect.Top ), generator(_generator)
{
    L_( "making chunk for %d,%d - %d,%d\n", rect.Left, rect.Top, rect.Right, rect.Bottom );

    // Fill up with stars
    float area = std::abs( rect.Left - rect.Right ) * std::abs( rect.Top - rect.Bottom );

    L_ << TWEAKS->GetNum( "star_density" ) << '\n';
    const size_t num_stars = area * TWEAKS->GetNum( "star_density" );

    L_( "area %f => %d stars\n", area, num_stars );

    // Set some stars
    for( size_t i = 0; i < num_stars; ++i ) {
        stars.push_back( generator.CreateStar( start_pos) );
    }

    // Generate some junk
    for( size_t i = 0; i < 4; ++i ) {
        items.push_back( generator.CreateJunk( start_pos ) );
    }
}

void Chunk::Draw( Vec2i offset )
{
    // Draw stars
    for( size_t i = 0; i < stars.size(); ++i ) {
        stars[i].Draw( offset );
    }

    // Draw items
    for( size_t i = 0; i < items.size(); ++i ) {
        items[i]->Draw( offset );
    }
}

