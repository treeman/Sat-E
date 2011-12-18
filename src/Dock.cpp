#include "Dock.hpp"

Dock::Dock( Satellite &sat ) : satellite(sat)
{
    str = BUTLER->CreateString( "fnt/consola.ttf", 15 );
    str.SetColor( Tree::Color( 0xff00ff00 ) );

    Deactivate();

    // Init selections
    AddSelection( "Battery", Battery );
    AddSelection( "Acceleration", Acceleration );
    AddSelection( "Speed", Speed );
    AddSelection( "Metal plating", Armor );
    AddSelection( "Exchange" );

    AddSelection( "Reciever", Reciever );
    AddSelection( "Teleport", Teleport );
    AddSelection( "Coke-Hat" );
    AddSelection( "Girlfriend" );
    AddSelection( "Close" );

    bad_snd = BUTLER->CreateSound( "snd/bad_choice.wav" );
    select_snd = BUTLER->CreateSound( "snd/Powerup.wav" );
    move_snd = BUTLER->CreateSound( "snd/Blip_Select.wav" );
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
    // Magic :o scary shit...
    selections[6].available = true;
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
            Tree::Draw( sf::Shape::Rectangle( xp, yp, xp + w, yp + h, Tree::Color( 0xffffffff ) ) );

            const int i = x + y * rows;
            str.SetText( selections[i].txt );
            str.SetPosition( xp, yp );
            if( i == curr_selection ) {
                str.SetColor( Tree::Color( 0xffff0000 ) );
            }
            else {
                str.SetColor( Tree::Color( 0xff00ff00 ) );
            }
            Tree::Draw( str );
        }
    }

    str.SetText( selections[curr_selection].txt );
    str.SetPosition( 300, 250 );
    Tree::Draw( str );
}

void Dock::AddSelection( std::string txt, Action action )
{
    Selection selection;
    selection.txt = txt;
    selection.action = action;
    selections.push_back( selection );
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
        if( s.action != Exit ) select_snd.Play();

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
            default:
                break;
        }
    }
    else {
        bad_snd.Play();
    }
}

