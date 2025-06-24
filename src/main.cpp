#include <raylib.h>
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding rand()
#include <string>
#include <iostream>
#include <functional>
#include <chrono>

// Settings
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const Color BG_COLOR = { 15, 10, 25, 255 };
const float PLAYER_SPEED = 500.0f;
const float LASER_SPEED = 600.0f;
const float METEOR_SPEED_MIN = 300.0f;
const float METEOR_SPEED_MAX = 400.0f;
const float METEOR_TIMER_DURATION = 0.4f;
const float FONT_SIZE = 120.0f;

// Random helper functions
int randint(int min, int max) {
    return min + (rand() % (max - min + 1));
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

int main() {
    InitWindow(800, 600, "Timer Seconds Test");
    SetTargetFPS(60);

    srand(static_cast<unsigned int>(time(nullptr))); // seed random

    Timer myTimer(1.0f, true, true, onTimerExpire);  // 1-second timer, repeats, autostart

    while (!WindowShouldClose()) {
        myTimer.update();

        BeginDrawing();
        ClearBackground(BG_COLOR);
        DrawText("Watch the console: Timer fires every 1 second!", 20, 20, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
