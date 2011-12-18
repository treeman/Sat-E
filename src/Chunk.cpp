#include "Chunk.hpp"

bool can_kill( ItemPtr item )
{
    return item->CanKill();
}

void DrawOutline( sf::IntRect box, Vec2i offset )
{
    sf::Shape border = sf::Shape::Rectangle( box.Left, box.Top, box.Right, box.Bottom,
        Tree::Color( 0xFFFFFFFF ), 1.0, Tree::Color( 0xFFFFFFFF ) );
    border.EnableFill( false );
    border.EnableOutline( true );

    border.Move( offset );

    Tree::Draw( border );
}

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

    // Draw outlines
    if( SETTINGS->GetValue<bool>( "bounding_box_show" ) ) {
        for( Items::iterator it = items.begin(); it != items.end(); ++it ) {
            DrawOutline( (*it)->BoundingBox(), offset );
        }
    }
}

