
#include "Chunk.hpp"

Chunk::Chunk( sf::IntRect rect ) : start_pos( rect.Left, rect.Top )
{
    L_( "making chunk for %d,%d - %d,%d\n", rect.Left, rect.Top, rect.Right, rect.Bottom );

    // Init star resource
    star_spr = BUTLER->CreateSprite( "gfx/star.png" );

    float area = std::abs( rect.Left - rect.Right ) * std::abs( rect.Top - rect.Bottom );

    L_ << TWEAKS->GetNum( "star_density" ) << '\n';
    const size_t num_stars = area * TWEAKS->GetNum( "star_density" );

    L_( "area %f => %d stars\n", area, num_stars );

    // Set some stars
    for( size_t i = 0; i < num_stars; ++i ) {
        Star star;
        star.pos = Vec2i( math::irandom( rect.Left, rect.Right ), math::irandom( rect.Top, rect.Bottom ) );
        star.power = math::frandom( 0, 0.4 );

        //star.color = Tree::Color( *math::random( star_colors.begin(), star_colors.end() ) );
        star.color = 0xFF336699;

        stars.push_back( star );
    }
}

void Chunk::Draw( Vec2i offset )
{
    // Draw stars
    for( size_t i = 0; i < stars.size(); ++i ) {
        Vec2i pos = stars[i].pos + offset;

        star_spr.SetPosition( pos );

        Tree::Color col = stars[i].color;
        col.a = 0xFF * ( stars[i].power + math::frandom( 0, 0.1 ) );
        star_spr.SetColor( col );

        Tree::Draw( star_spr );
    }
}

