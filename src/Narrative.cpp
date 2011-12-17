#include "Narrative.hpp"

Line::Line( std::string _str, float time ) : str(_str), alive_time(time)
{
    t.Start();
    r_str = BUTLER->CreateString( "fnt/arial.ttf", 12 );
}

bool Line::IsDone()
{
    return t.GetTime() > alive_time;
}

void Line::DrawAt( Vec2i pos )
{
    if( !IsDone() ) {
        r_str.SetPosition( pos );
        Tree::Draw( r_str );
    }
}

Narrative::Narrative()
{

}

void Narrative::SetPos( Vec2f _pos )
{
    pos = _pos;
}

void Narrative::Draw()
{

}

