#include "ItemGenerator.hpp"

Star::Star( Vec2i _pos, Tree::Color _color ) : pos(_pos), color(_color)
{
    power = math::frandom( 0, 0.4 );

    spr = BUTLER->CreateSprite( "gfx/star.png" );
}

void Star::Draw( Vec2i offset )
{
    spr.SetPosition( pos + offset );

    // Flicker
    Tree::Color col = color;
    col.a = 0xFF * ( power + math::frandom( 0, 0.1 ) );
    spr.SetColor( col );

    Tree::Draw( spr );
}

ItemGenerator::ItemGenerator( int ch, int cw ) : chunk_w(cw), chunk_h(ch)
{
    // 'Random' positions
    for( int x = 0; x < chunk_w; x += 5 ) {
        for( int y = 0; y < chunk_h; y += 5 ) {
            position_bag.Add( Vec2i( x, y ) );
        }
    }

    // Get a list of junk apperances
    Tree::ImgPtr img = BUTLER->GetImage( "gfx/junk.png" );

    const int width = 20;
    const int height = 20;

    const int num_w = img->GetWidth() / width;
    const int num_h = img->GetHeight() / height;
    const int images = num_w * num_h;

    //L_( "%dx%d = %d\n", img->GetWidth(), img->GetHeight(), images );

    // Place all subsprites in a bag so we can generate randomly
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

ItemPtr ItemGenerator::CreateJunk( Vec2i offset )
{
    ItemPtr junk( new Junk( junk_bag.Get() ) );
    junk->SetPos( position_bag.Get() + offset );
    return junk;
}

Star ItemGenerator::CreateStar( Vec2i offset )
{
    Vec2i pos = Vec2i( math::irandom( 0, chunk_w ), math::irandom( 0, chunk_h ) );
    pos += offset;

    //star.color = Tree::Color( *math::random( star_colors.begin(), star_colors.end() ) );
    Tree::Color color = 0xFF336699;

    return Star( pos, color );
}

