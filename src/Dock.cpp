#include "Dock.hpp"

Dock::Dock( Satellite &sat ) : satellite(sat)
{
    str = BUTLER->CreateString( "fnt/consola.ttf", 15 );
    str.SetColor( Tree::Color( 0xff00ff00 ) );

    Reset();

    bad_snd = BUTLER->CreateSound( "snd/bad_choice.wav" );
    select_snd = BUTLER->CreateSound( "snd/Powerup.wav" );
    move_snd = BUTLER->CreateSound( "snd/Blip_Select.wav" );

    junk_spr = BUTLER->CreateSprite( "junk" );
    coveted_spr = BUTLER->CreateSprite( "coveted" );
}

void Dock::Reset()
{
    Deactivate();

    // Init selections in a slightly ugly way ^^
    for( int i = 0; i < NumActions; ++i ) {
        Selection selection;
        selection.action = (Action)i;
        selections.push_back( selection );
    }

    selections[Battery].txt = "Some more batteries so I can explore more.";
    selections[Battery].junk_cost = 2;
    selections[Battery].spr = BUTLER->CreateSprite( "battery" );

    selections[Acceleration].txt = "Better handling, better controll.";
    selections[Acceleration].junk_cost = 20;
    selections[Acceleration].spr = BUTLER->CreateSprite( "acc" );

    selections[Speed].txt = "Rockets! Shoot me forward!";
    selections[Speed].junk_cost = 20;
    selections[Speed].spr = BUTLER->CreateSprite( "rocket" );

    selections[Armor].txt = "Mmmh... Will this make me hard and cool?";
    selections[Armor].junk_cost = 20;
    selections[Armor].spr = BUTLER->CreateSprite( "armor" );

    selections[Exchange].txt = "Trade good for bad? I'm a hard negotiator...";
    selections[Exchange].coveted_cost = 1;
    selections[Exchange].spr = BUTLER->CreateSprite( "exchange" );

    selections[Reciever].txt = "Is this a GPS? Or a radar? How does that work?";
    selections[Reciever].junk_cost = 40;
    selections[Reciever].spr = BUTLER->CreateSprite( "phonehome" );

    selections[Teleport].txt = "Beam me in Scotty! Where's Scotty anyway?";
    selections[Teleport].junk_cost = 50;
    selections[Teleport].spr = BUTLER->CreateSprite( "teleporter" );

    selections[CokeHat].txt = "'Beer-On-A-Cap'. Oh My God!!";
    selections[CokeHat].coveted_cost = 20;
    selections[CokeHat].spr = BUTLER->CreateSprite( "beercap" );

    selections[Friend].txt = "Make a friend. Not that I'm lonely.";
    selections[Friend].coveted_cost = 100;
    selections[Friend].spr = BUTLER->CreateSprite( "friendz" );

    selections[Exit].txt = "Go away from docking.";
    selections[Exit].spr = BUTLER->CreateSprite( "exit" );
}

bool Dock::IsActive()
{
    return is_active;
}
void Dock::Activate()
{
    is_active = true;
}
void Dock::Deactivate()
{
    is_active = false;
    curr_selection = 0;
}

bool Dock::HandleEvent( sf::Event &e )
{
    if( e.Type == sf::Event::KeyPressed ) {
        switch( e.Key.Code ) {
            case sf::Key::Escape:
                Deactivate();
                break;
            case sf::Key::Up:
                MoveUp();
                break;
            case sf::Key::Down:
                MoveDown();
                break;
            case sf::Key::Left:
                MoveLeft();
                break;
            case sf::Key::Right:
                MoveRight();
                break;
            case sf::Key::Return:
            case sf::Key::Space:
                Execute();
                break;
            default:
                break;
        }
    }
    return true;
}

void Dock::AddTeleport()
{
    selections[Teleport].available = true;
}

void Dock::Update( float dt )
{

}
void Dock::Draw()
{
    const Vec2i topleft( 200, 100 );
    const int x_space = 20;
    const int y_space = 20;

    const int w = 60;
    const int h = 60;

    const int rows = 5;
    const int lines = 2;

    for( int x = 0; x < rows; ++x ) {
        for( int y = 0; y < lines; ++y ) {
            const int xp = topleft.x + (w + x_space) * x;
            const int yp = topleft.y + (w + y_space) * y;

            const int index = x + y * rows;

            Tree::Color col;
            if( curr_selection == index ) { col = Tree::Color( 0xff4F0202 ); }
            else { col = Tree::Color( 0xff242424 ); }

            Tree::Draw( sf::Shape::Rectangle( xp, yp, xp + w, yp + h, Tree::Color( 0xff454545 ), 3.0, col ) );

            Selection &s = selections[index];
            s.spr.SetPosition( xp, yp );
            if( index != curr_selection ) {
                s.spr.SetColor( Tree::Color( 0xff666666 ) );
            }
            else if( !s.available ) {
                s.spr.SetColor( Tree::Color( 0xff333333 ) );
            }
            else {
                s.spr.SetColor( Tree::Color( 0xffffffff ) );
            }
            Tree::Draw( s.spr );
        }
    }

    Selection &s = selections[curr_selection];

    str.SetColor( Tree::Color( 0xffcccccc ) );
    str.SetText( selections[curr_selection].txt );
    str.SetPosition( 200, 250 );
    Tree::Draw( str );

    if( s.junk_cost > 0 ) {
        junk_spr.SetPosition( 200, 270 );
        Tree::Draw( junk_spr );

        str.SetColor( Tree::Color( 0xffcccccc ) );
        str.SetPosition( 230, 272 );
        str.SetText( boost::lexical_cast<std::string>( s.junk_cost ) );
        Tree::Draw( str );
    }
    else if( s.coveted_cost > 0 ) {
        coveted_spr.SetPosition( 200, 272 );
        Tree::Draw( coveted_spr );

        str.SetColor( Tree::Color( 0xffcccccc ) );
        str.SetPosition( 230, 272 );
        str.SetText( boost::lexical_cast<std::string>( s.coveted_cost ) );
        Tree::Draw( str );
    }
}

// rows = 5 magic variable :)
void Dock::MoveUp()
{
    curr_selection -= 5;
    if( curr_selection < 0 ) curr_selection += selections.size();
    move_snd.Play();
}
void Dock::MoveDown()
{
    curr_selection += 5;
    if( curr_selection >= (int)selections.size() ) curr_selection -= selections.size();
    move_snd.Play();
}
void Dock::MoveLeft()
{
    --curr_selection;
    if( curr_selection < 0 ) curr_selection = selections.size() - 1;
    move_snd.Play();
}
void Dock::MoveRight()
{
    ++curr_selection;
    if( curr_selection >= (int)selections.size() ) curr_selection = 0;
    move_snd.Play();
}
void Dock::Execute()
{
    if( selections[curr_selection].available ) {
        Selection &s = selections[curr_selection];

        if( !MeetsDemand( s ) ) {
            bad_snd.Play();
        }
        else {
            if( s.action != Exit ) select_snd.Play();

            RetractDemand( s );

            switch( s.action ) {
                case Exit:
                    Deactivate();
                    break;
                case Battery:
                    satellite.IncrMaxFuel( TWEAKS->GetNum( "fuel_incr" ) );
                    if( ++s.count >= 5 ) s.available = false;
                    break;
                case Acceleration:
                    satellite.IncrBoost();
                    s.available = false;
                    break;
                case Speed:
                    satellite.IncrSpeed( TWEAKS->GetNum( "speed_incr" ) );
                    if( ++s.count >= 2 ) s.available = false;
                    break;
                case Armor:
                    satellite.IncrArmor( TWEAKS->GetNum( "armor_incr" ) );
                    if( ++s.count >= 2 ) s.available = false;
                    break;
                case Reciever:
                    satellite.AddonWayHome();
                    s.available = false;
                    break;
                case Teleport:
                    satellite.AddTeleport();
                    s.available = false;
                    break;
                case Friend:
                    satellite.AddFriend();
                    s.available = false;
                    Deactivate();
                    break;
                case CokeHat:
                    satellite.AddBeerCap();
                    s.available = false;
                    break;
                case Exchange:
                    satellite.ChangeJunk( 10 );
                    break;
                default:
                    break;
            }
        }
    }
    else {
        bad_snd.Play();
    }
}

bool Dock::MeetsDemand( Selection &s )
{
    const int junk = satellite.JunkCollected();
    const int coveted = satellite.CovetedCollected();

    return junk >= s.junk_cost && coveted >= s.coveted_cost;
}

void Dock::RetractDemand( Selection &s )
{
    satellite.ChangeJunk( -s.junk_cost );
    satellite.ChangeCoveted( -s.coveted_cost );
}

