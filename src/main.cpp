#include <raylib.h>
#include <raymath.h> 
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding rand()
#include <string>
#include <iostream>
#include <functional>
#include <chrono>
using namespace std;



// Settings
const int WINDOW_WIDTH = 800; //1920
const int WINDOW_HEIGHT = 600; //1080
const Color BG_COLOR = { 15, 10, 25, 255 };
const float PLAYER_SPEED = 500.0f;
const float LASER_SPEED = 600.0f;
const float METEOR_SPEED_MIN = 300.0f;
const float METEOR_SPEED_MAX = 400.0f;
const float METEOR_TIMER_DURATION = 0.4f;
const float FONT_SIZE = 120.0f;

// Random helper functions
float randint(float min, float max) {
    return min + (rand() % (int)(max - min + 1));
}

float uniform(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * (max - min);
}

// Helper to get current time in seconds (float)
inline float get_time() {
    using namespace std::chrono;
    return std::chrono::duration_cast<std::chrono::duration<float>>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

// Timer class
class Timer {
public:
    Timer(float durationSec, bool repeat = false, bool autostart = false, std::function<void()> func = nullptr)
        : duration(durationSec), repeat(repeat), func(func), active(false), start_time(0.0f)
    {
        if (autostart) {
            activate();
        }
    }

    void activate() {
        active = true;
        start_time = get_time();
    }

    void deactivate() {
        active = false;
        start_time = 0.0f;
        if (repeat) {
            activate();
        }
    }

    void update() {
        if (active) {
            if (get_time() - start_time >= duration) {
                if (func && start_time != 0.0f) {
                    func();
                }
                deactivate();
            }
        }
    }

private:
    float duration;                   // Duration in seconds
    float start_time;                 // Start time in seconds
    bool active;
    bool repeat;
    std::function<void()> func;
};

// Callback for timer
void onTimerExpire() {
    std::cout << "Timer expired!" << std::endl;
}

class Sprite{
    public:
    Texture2D texture;
    Vector2 pos;
    float speed;
    Vector2 direction;
    Vector2 size; //= { (float)texture.width, (float)texture.height };

    Sprite(){

    }
    Sprite(Texture2D t, Vector2 p, float s, Vector2 d){
        texture = t;
        pos = p;
        speed = s;
        direction = d;
    }
    Sprite(Texture2D t, Vector2 p, float s){
        texture = t;
        pos = p;
        speed = s;
    }
    void move(float dt){
        pos.x += direction.x * speed * dt;
        pos.y += direction.y * speed * dt;
    }
    void update(float dt){

    }
    void draw(void){
        DrawTextureV(texture,pos,WHITE);
    }
};

class Player: public Sprite{
    public:
    Player(){
        speed = PLAYER_SPEED;
    }
    Player(Texture2D t, Vector2 p): Sprite(t,p, PLAYER_SPEED){

    }
    void input(void){
        direction.x = (int)(IsKeyDown(KEY_RIGHT)) - (int)(IsKeyDown(KEY_LEFT));
        direction.y = (int)(IsKeyDown(KEY_DOWN)) - (int)(IsKeyDown(KEY_UP));
        if (direction.x != 0 || direction.y != 0) {
            direction = Vector2Normalize(direction);
        }
        if((int)(IsKeyPressed(KEY_SPACE))){
            cout<<"Laser"<<endl;
        }
    }
    void constraint(void){
        pos.x = Clamp(pos.x, 0, (WINDOW_WIDTH - size.x));
        pos.y = Clamp(pos.y, 0, (WINDOW_HEIGHT - size.y));
    }
    void update(float dt){
        input();
        move(dt);
        constraint();
    }
};

class Star : public Sprite {
    public:
    float scale;
    void draw(void){
        DrawTextureEx(texture,pos,0,scale,WHITE);
    }

};

class Game{
    public:
    Player player;
    Star star[30];
    Game(){
        InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Asteroids");
        player.texture = LoadTexture("images/spaceship.png");
        player.pos = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
        player.size = { (float)player.texture.width, (float)player.texture.height };
        Texture2D starTexture = LoadTexture("images/star.png");
        for(int i =0; i<30; i++){
            star[i].texture = starTexture;
            star[i].pos = {randint(0,WINDOW_WIDTH),randint(0,WINDOW_HEIGHT)};
            star[i].scale = uniform(0.5, 1.2); 
        }

    }
    void update(void){
        float dt = GetFrameTime();
        player.update(dt);
    }
    void draw(void){
        BeginDrawing();
        ClearBackground(BG_COLOR);
        for(int i =0; i<30; i++){
            star[i].draw();
        }
        player.draw();
        EndDrawing();
    }

    void run(void){
        while(!WindowShouldClose()){
            update();
            draw();
            
        }
        CloseWindow();
    }
};

int main() {
    Game game;
    game.run(); 

    return 0;
}
