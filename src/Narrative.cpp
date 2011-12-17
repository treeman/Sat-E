#include "Narrative.hpp"

Line::Line( std::string _str, float pause, float display ) : str(_str), alive_time(display), pause_before(pause)
{
}

Line::Line( const Line &line )
{
    str = line.str;
    alive_time = line.alive_time;
    pause_before = line.pause_before;
}

void Line::Start()
{
    t.Start();
}

bool Line::IsDone()
{
    return t.GetTime() > alive_time + pause_before;
}

const std::string Line::Str()
{
    if( t.GetTime() < pause_before || IsDone() ) {
        return "";
    }
    else {
        return str;
    }
}

Talk::Talk()
{
}

Talk::Talk( const Talk &talk )
{
    for( size_t i = 0; i < talk.lines.size(); ++i ) {
        Line line = Line( talk.lines[i] );
        lines.push_back( line );
    }
}

void Talk::Start()
{
    if( !lines.empty() ) {
        lines[0].Start();
    }
}

bool Talk::IsDone()
{
    Update();
    return lines.empty();
}

void Talk::Push( std::string str, float alive, float pause_before )
{
    lines.push_back( Line( str, alive, pause_before ) );
}

std::string Talk::Get()
{
    if( IsDone() ) {
        return "";
    }
    else {
        return lines[0].Str();
    }
}

void Talk::Update()
{
    if( lines.empty() ) return;

    if( lines[0].IsDone() ) {
        lines.erase( lines.begin() );
    }
    Start();
}

Narrative::Narrative()
{
    // Parse file for narratives
    ParseFile( "narrative" );

    // Add in delay between talks
    delays.Add(10).Add(15).Add(30).Add(40).Add(60).Add(15).Add(100);
    //delays.Add(1).Add(2).Add(3);

    // Init narrative
    curr_delay = delays.Get();
    Update();

    t.Start();

    r_str = BUTLER->CreateString( "fnt/arial.ttf", 15 );
}

void Narrative::SetPos( Vec2i _pos )
{
    pos = _pos;
}

void Narrative::Draw()
{
    Update();

    r_str.SetText( curr_talk.Get() );
    r_str.SetPosition( pos.x + 5, pos.y - 35 );
    Tree::Draw( r_str );
}

void Narrative::ParseFile( std::string file )
{
    std::ifstream in( file.c_str() );

    if( !in.is_open() ) {
        std::string s = "Error opening: " + file;
        throw( Tree::resource_not_found( s.c_str() ) );
    }

    Talk talk;

    while( !in.eof() )
    {
        std::string str;
        std::getline( in, str );

        // Continue to next talk at newline
        if( str.size() == 0 ) {
            // If we have anything then insert
            if( !talk.IsDone() ) {
                //talks.push_back( talk );
                talks.Add( talk );
            }
            talk = Talk();
            continue;
        }

        // split string into
        // <pause_before> <display_time> <text...>
        // eg '0 2 hi you' => 0, 2, "hi you"
        // where is regex when you need them!? oh perl...
        int p = str.find( ' ' );

        const std::string pause = str.substr( 0, p );
        str = str.substr( p + 1 ); // rest is now str

        p = str.find( ' ' );
        const std::string display = str.substr( 0, p );
        str = str.substr( p + 1 ); // str is now our text to be displayed

        const int pause_time = boost::lexical_cast<int>( pause );
        const int display_time = boost::lexical_cast<int>( display );

        talk.Push( str, pause_time, display_time );
    }
}

void Narrative::Update()
{
    if( curr_talk.IsDone() ) {
        if( t.GetTime() > curr_delay ) {
            curr_talk = Talk( talks.Get() );
            curr_delay = delays.Get();
            t.Stop();
            curr_talk.Start();
        }
        else if( !t.IsStarted() ) {
            t.Start();
        }
    }
}

