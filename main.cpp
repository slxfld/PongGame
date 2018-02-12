#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <sstream>




namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

class ent{
public:
    ent(){}
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 2;
    void draw(sf::RenderWindow &window){
        sprite.setScale(sf::Vector2f(1,1));
        window.draw(sprite);
    }
};
class pad : public ent{
    public:
        pad(sf::Texture &_texture,int xpos,int ypos){
            texture = _texture;
            sprite.setTexture(texture);
            sprite.setPosition(sf::Vector2f(xpos,ypos));
        }
    void movepad(int Dir,int bally){

        if(sprite.getPosition().y < 10){
            sprite.move(sf::Vector2f(0,speed));
        }
        if(sprite.getPosition().y + sprite.getGlobalBounds().height > 200-10){
            sprite.move(sf::Vector2f(0,-speed));
        }


        if(isplayer){
            if(Dir==1){
                sprite.move(sf::Vector2f(0,-speed));
            }
            if(Dir==2){
                sprite.move(sf::Vector2f(0,speed));
            }
        }
        else
        {
            if(bally > (sprite.getPosition().y + sprite.getGlobalBounds().height/2) )
            {
                sprite.move(sf::Vector2f(0,speed));
            }
            if(bally < (sprite.getPosition().y + sprite.getGlobalBounds().height/2) )
            {
                sprite.move(sf::Vector2f(0,-speed));
            }
        }
    }
    void update(){
        if(moving){
            movepad(direction,0);
        }
    }

    bool isplayer=true;
    int direction=0;
    bool moving = false;
};




class ball : public ent{
    public:
        ball(sf::Texture &_texture,int xpos,int ypos){
            texture = _texture;
            sprite.setTexture(texture);
            sprite.setPosition(sf::Vector2f(xpos,ypos));
            startx = xpos;
            starty = ypos;
        }

    void update(int width, int height,int &points,int &e_points){
        sprite.move(sf::Vector2f(speed,yspeed));
        if(counter>0){counter--;}

        if(sprite.getPosition().y + sprite.getGlobalBounds().height > height - 10||sprite.getPosition().y < 10){
            yspeed = -yspeed;
        }
        if(sprite.getPosition().y + sprite.getGlobalBounds().height > height - 10){
                sprite.move(sf::Vector2f(0,-2));
        }

        if(sprite.getPosition().x > width){
            points++; std::cout << "Player +1\n";
            sprite.setPosition(sf::Vector2f(startx,starty));
            yspeed = speed;
        }
        else if(sprite.getPosition().x < 0){
            e_points++; std::cout << "Computer +1\n";
            sprite.setPosition(sf::Vector2f(startx,starty));
            yspeed = speed;
        }

        if(trail_counter>0){trail_counter--;}
        if(trail_counter==-1){trail_counter=10;}

    }
        double fRand(double fMin, double fMax){
            double f = (double)rand() / RAND_MAX;
            return fMin + f * (fMax - fMin);
        }
    void check_collision(sf::Sprite c_sprite,bool isplayer){
        bool stop = false;
        if(counter==0){

        if(!isplayer)
        {
            if(sprite.getPosition().x+sprite.getGlobalBounds().width > c_sprite.getPosition().x+5
               &&sprite.getPosition().y+sprite.getGlobalBounds().height>c_sprite.getPosition().y){
                stop = true;
            }
            if(sprite.getPosition().x+sprite.getGlobalBounds().width > c_sprite.getPosition().x+5
               &&sprite.getPosition().y<c_sprite.getPosition().y+c_sprite.getGlobalBounds().height){
                stop = true;
            }
        }
        else{
            if(sprite.getPosition().x < c_sprite.getPosition().x +c_sprite.getGlobalBounds().width-5
               &&sprite.getPosition().y+sprite.getGlobalBounds().height>c_sprite.getPosition().y){
                stop = true;
            }
            if(sprite.getPosition().x < c_sprite.getPosition().x +c_sprite.getGlobalBounds().width-5
               &&sprite.getPosition().y<c_sprite.getPosition().y+c_sprite.getGlobalBounds().height){
                stop = true;
            }
        }




            if(stop==false){
                if(sprite.getGlobalBounds().intersects(c_sprite.getGlobalBounds())){
                    speed = -speed;
                    srand(time(NULL));
                    double randnum = rand() % 90;
                    yspeed = 1 + randnum / 10;
                    std::cout << "speed :"<< yspeed << "\n";
                    counter = 25;
                }
            }
        }
    }

    float yspeed=speed;
    int startx;
    int starty;
    int counter = 0;
    int trail_counter = 0;
};


class trail{
public:
    trail(){ }

    void draw(sf::RenderWindow &window){
        sf::RectangleShape rect(sf::Vector2f(7,7));
        rect.setPosition(pos);
        rect.setFillColor(sf::Color(0,255,0,20));
        window.draw(rect);

        rem_count++;
        if(rem_count>=20){
            remove_ = true;
        }
    }
    sf::Vector2f pos;

    bool remove_=false;
    int rem_count = 0;
};


int main()
{
    int WIDTH = 400;
    int HEIGHT = 200;

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT),"pong");
    window.setFramerateLimit(50);


//Resources
    sf::Texture pad_texture;
    sf::Texture pad_texture2;

    sf::Texture ball_texture;
    sf::Texture background;

//FONT
    sf::Font font;
    font.loadFromFile("DATA/FONT.ttf");

//SCORE TEXT
    sf::Text score;
    score.setFont(font);
    score.setPosition(sf::Vector2f(WIDTH/2 -30,5));
    score.setCharacterSize(35);

    pad_texture.loadFromFile("DATA/pad.png");
    pad_texture2.loadFromFile("DATA/e_pad.png");
    ball_texture.loadFromFile("DATA/ball.png");
    background.loadFromFile("DATA/bg.png");

//Player, Enemy
    pad player_pad(pad_texture,50,70);
    pad enemy_pad(pad_texture2,WIDTH-50-12,70);

    enemy_pad.isplayer=false;

    sf::Sprite backg;
    backg.setTexture(background);

//Ball
    ball ball1(ball_texture,window.getSize().x/2,50);


//Trail
    std::list<trail*> trails;

    int points = 0;
    int e_points = 0;

    while(window.isOpen()){

        sf::Event event;

        while(window.pollEvent(event)){

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                player_pad.direction = 1;
                player_pad.moving = true;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                player_pad.direction = 2;
                player_pad.moving = true;
            }
            else{
                player_pad.moving = false;
            }

            if(event.type == sf::Event::Closed){
                window.close();
            }
        }



        //Trails Spawn
        if(ball1.trail_counter==0){
            trail* trl = new trail();
            trl->pos = (ball1.sprite.getPosition());
            trails.push_back(trl);
        }

        //Trails Memory
        for(trail* t : trails){
            if(t->remove_==true){
                trails.remove(t);
                delete t; break;
            }
        }

        window.clear();
//_______________________________________________________________
        window.draw(backg);

        for(trail* t : trails){ t->draw(window); }

        ball1.draw(window);
        ball1.update(WIDTH,HEIGHT,points,e_points);
        ball1.check_collision(player_pad.sprite,player_pad.isplayer);
        ball1.check_collision(enemy_pad.sprite,enemy_pad.isplayer);

        player_pad.draw(window);
        player_pad.update();

        enemy_pad.draw(window);
        enemy_pad.movepad(9,ball1.sprite.getPosition().y);

        std::string st1 = patch::to_string(points);
        std::string st2 = patch::to_string(e_points);
        std::string score_string = st1 + ":" + st2;
        score.setString(score_string);

        window.draw(score);
//_______________________________________________________________
        window.display();
    }


    return 0;
}
