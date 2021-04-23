#include <SFML/Graphics.hpp>
#include <ctime>
#include <list>
#include<chrono>
#include<thread>
using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

class Animation
{
public:
    float Frame, speed;
    Sprite sprite;
    std::vector<IntRect> frames;

    Animation() {}

    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }


    void update()
    {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) Frame -= n;
        if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
    }

    bool isEnd()
    {
        return Frame + speed >= frames.size();
    }

};


class Entity
{
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;
    Animation anim;

    Entity()
    {
        life = 1;
    }

    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update() {};

    void draw(RenderWindow& app)
    {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite);

        CircleShape circle(R);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(x, y);
        circle.setOrigin(R, R);
        //app.draw(circle);
    }

    virtual ~Entity() {};
};


class asteroid : public Entity
{
public:
    asteroid()
    {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
    }

    void update()
    {
        x += dx;
        y += dy;

        if (x > W) x = 0;  if (x < 0) x = W;
        if (y > H) y = 0;  if (y < 0) y = H;
    }

};


class bullet : public Entity
{
public:
    bullet()
    {
        name = "bullet";
    }

    void  update()
    {
        dx = cos(angle * DEGTORAD) * 6;
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%7-3;  /*try this*/
        x += dx;
        y += dy;

        if (x > W || x<0 || y>H || y < 0) life = 0;
    }

};


class player : public Entity
{

public:
    int health_points;
    bool thrust;
    int points;
    player()
    {
        name = "player";
        health_points = 3;
        points = 0;
    }

    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        else
        {
            dx *= 0.99;
            dy *= 0.99;
        }

        int maxSpeed = 15;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;

        if (x > W) x = 0; if (x < 0) x = W;
        if (y > H) y = 0; if (y < 0) y = H;
    }

};
class Health_Points {
public:
    Animation anim;
    float x, y, angle;
    Health_Points(Animation& a, int X, int Y, float Angle = 0)
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
       // R = radius;
    }
    void draw(RenderWindow& app)
    {
        anim.sprite.setPosition(x, y);
        anim.sprite.setRotation(angle + 90);
        app.draw(anim.sprite);
    }
};
//class Game_Points {
//public:
//    Text points_;
//    float x, y, angle;
//   Game_Points( int X, int Y, float Angle = 0)
//   {
//       Font font;
//       font.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/Chunk Five Print.otf");
//        x = X; y = Y;
//        angle = Angle;
//        points_.setFont(font);
//        points_.setFillColor(Color(255, 21, 25, 40));
//        points_.setOutlineColor(Color::Black);
//        points_.setOutlineThickness(2);
//        points_.setCharacterSize(48);
//        points_.setStyle(sf::Text::Regular);
//        points_.setString(std::to_string(0));
//    }
//    void draw(RenderWindow& app)
//    {
//        points_.setPosition(x, y);
//        app.draw(points_);
//    }
//};

bool isCollide(Entity* a, Entity* b)
{
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}


int main()
{
    int death_flag = false;
    srand(time(0));

    Text points_;
    Font font;
    font.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/Chunk Five Print.otf");
    points_.setFont(font);
    points_.setFillColor(Color(255, 0, 0, 200));
    points_.setOutlineColor(Color::Black);
    points_.setOutlineThickness(2);
    points_.setCharacterSize(48);
    points_.setStyle(sf::Text::Regular);
    points_.setString(std::to_string(0));
    points_.setPosition(20, 0);

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9;
    t1.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/spaceship.png");
    t2.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/background.jpg");
    t3.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/explosions/type_C.png");
    t4.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/rock.png");
    t5.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/fire_blue.png");
    t6.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/rock_small.png");
    t7.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/explosions/type_B.png");
    t8.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/health_points.png");
    t9.loadFromFile("C:/workspace/cpp/asteroids/asteroids/images/game_over.png");
    t1.setSmooth(true);
    t2.setSmooth(true);
    Sprite background(t2);
    Sprite game_over(t9);
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation health_points(t8, 0, 0, 225, 225, 1, 0);

    std::list<Entity*> entities;

    for (int i = 0; i < 15; i++)
    {
        asteroid* a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }

    player* p = new player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Space)
                {
                    bullet* b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
        if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;
        if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
        else p->thrust = false;


        for (auto a : entities)
            for (auto b : entities)
            {
                if (a->name == "asteroid" && b->name == "bullet")
                    if (isCollide(a, b))
                    {
                        a->life = false;
                        b->life = false;
                        p->points++;
                        points_.setString(std::to_string(p->points));
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);


                        for (int i = 0; i < 2; i++)
                        {
                            if (a->R == 15) continue;
                            Entity* e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                            entities.push_back(e);
                        }

                    }

                if (a->name == "player" && b->name == "asteroid")
                    if (isCollide(a, b))
                    {
                        b->life = false;
                        Entity* e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        if (p->health_points > 1)
                        {
                            p->settings(sPlayer, W / 2, H / 2, 0, 20);
                            p->dx = 0; p->dy = 0;
                            p->health_points--;
                            
                        }
                        else 
                        {
                            p->health_points--;
                            for (int i = 0; i < 64; i++) {
                                app.draw(background);
                                e->anim.update();
                                e->draw(app);
                                app.display();
                            }
                            app.draw(background);
                            app.display();
                            death_flag = true;
                        }
                    }
            }
        if (death_flag) {
            
            app.clear();
            app.draw(background);
            for (int i = 0; i < 255; i = i + 7) {
               
                game_over.setColor(Color(255, 255, 255, i));
                app.draw(game_over);
                app.display();
            }
            system("pause");
            return 0;
        }
        if (p->thrust)  p->anim = sPlayer_go;
        else   p->anim = sPlayer;


        for (auto e : entities)
            if (e->name == "explosion")
                if (e->anim.isEnd()) e->life = 0;

        if (rand() % 150 == 0)
        {
            asteroid* a = new asteroid();
            a->settings(sRock, 0, rand() % H, rand() % 360, 25);
            entities.push_back(a);
        }

        for (auto i = entities.begin(); i != entities.end();)
        {
            Entity* e = *i;

            e->update();
            e->anim.update();

            if (e->life == false) { i = entities.erase(i); delete e; }
            else i++;
        }
       
        //////draw//////
        app.draw(background);
        for (auto i : entities) i->draw(app);
        
        for (auto i = 0; i < p->health_points; i++) {
            Health_Points* hp = new Health_Points(health_points, 1050+i*75, 120, -90);
            hp->draw(app);

        }
        //game_points.draw(app);
        app.draw(points_);
        app.display();
        
    }
   
    return 0;
}
