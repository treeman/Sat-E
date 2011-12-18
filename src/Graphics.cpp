#include "Graphics.hpp"

extern void draw_bar( sf::IntRect box, float perc, Tree::Color foreground,
    Tree::Color background, Tree::Color outline )
{
    if( perc > 1.0 ) perc = 1.0;
    if( perc < 0 ) perc = 0.0;

    const int w = std::abs( box.Left - box.Right );
    const int filled = w * perc;

    // Background + outline
    Tree::Draw( sf::Shape::Rectangle(
        box.Left, box.Top,
        box.Right, box.Bottom,
        background, 1.0, outline
    ));

    // Life
    Tree::Draw( sf::Shape::Rectangle(
        box.Left, box.Top,
        box.Left + filled, box.Bottom,
        foreground
    ));
}

extern void draw_outline( sf::IntRect box, Vec2i offset )
{
    sf::Shape border = sf::Shape::Rectangle( box.Left, box.Top, box.Right, box.Bottom,
        Tree::Color( 0xFFFFFFFF ), 1.0, Tree::Color( 0xFFFFFFFF ) );
    border.EnableFill( false );
    border.EnableOutline( true );

    border.Move( offset );

    Tree::Draw( border );
}

