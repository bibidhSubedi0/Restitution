#include <iostream>
#include <raylib.h>
#include <cmath>
#include <vector>
#include <string>
using std::cout, std::cin, std::endl, std::vector, std::string;

const int width = 1200;
const int height = 800;
const float deltaTime = 0.01;
float wind = 1;
#define pi 3.1415
#define g 1 // 10px per second = 1 px per second
#define coeffOfRestitution 0.8
#define coeffOfFriction 0.8
class particle
{
public:
    int radius;
    float direction;
    Vector2 position;
    Vector2 accleration;
    Vector2 velocity;

    particle(int radius, Vector2 position)
    {
        this->radius = radius;
        this->position = position;
        direction = pi / 2;
        velocity = {wind, 0};          // x-velocity is the component of wind
        accleration.x = 0.0;           // g*cos(direction) *deltaTime;
        accleration.y = g * deltaTime; //*sin(direction) *deltaTime;
    }
};
class Platform
{
public:
    Vector2 startPos;
    Vector2 endPos;

    Platform(Vector2 startPos, Vector2 endPos)
    {
        this->startPos = startPos;
        this->endPos = endPos;
    }
    void darwPlatfomr()
    {
        DrawLine(startPos.x, startPos.y, endPos.x, endPos.y, WHITE);
    }
};

void restitute(particle &part, Platform plat)
{
    part.position.y = plat.startPos.y - part.radius - 1;
    if (part.accleration.y > 0)
    {
        part.velocity.y = -part.velocity.y;
        part.velocity.y = part.velocity.y * coeffOfRestitution;
        part.velocity.x = part.velocity.x * coeffOfFriction ;
        if (fabs(part.velocity.y) < 0.5)
        {
            part.accleration.y = 0;
            part.velocity.y = 0;
        }
        // if(abs(part.velocity.x) < 0.001) part.velocity.x = 0;
    }
    return;
}

void moveParticle(particle &part, Platform plat)
{
    if (part.position.y + part.radius >= plat.startPos.y)
    {
        part.position.y = plat.startPos.y - part.radius;
        restitute(part, plat);
        return;
    }
    if (part.accleration.y == 0)
    {
        
        part.velocity.x = part.velocity.x * coeffOfFriction ;
    }

    part.velocity.y += part.accleration.y;

    part.position.x += part.velocity.x;
    part.position.y += part.velocity.y;
    if (part.position.x > width)
        part.position.x = 0;
    if (part.position.x < 0)
        part.position.x = width;
}
void logTexts(particle p, Platform plat)
{
    // DrawFPS(100, 100);
    string windy = std::to_string(wind);
    string velX = std::to_string(p.velocity.x);
    string vely = std::to_string(p.velocity.y);
    string height = std::to_string(abs(p.position.y - plat.startPos.y + p.radius));
    string cor = std::to_string(coeffOfRestitution);
    const char *w = windy.c_str();
    const char *vx = velX.c_str();
    const char *vy = vely.c_str();
    const char *h = height.c_str();
    const char *c = cor.c_str();
    DrawText(TextFormat("Wind: %3s", w), 10, 10, 20, WHITE);
    DrawText(TextFormat("Velocity X: %3s", vx), 10, 40, 20, WHITE);
    DrawText(TextFormat("Velocity Y: %3s", vy), 10, 70, 20, WHITE);
    DrawText(TextFormat("Height: %3s", h), 10, 100, 20, WHITE);
    DrawText(TextFormat("Coefficient of Restitution: %3s", c), 10, 130, 20, WHITE);
}

int main()
{
    SetTargetFPS(200);
    InitWindow(width, height, "Ref..");
    int ballRaddius = 2;
    particle bigCircle(5, {width / 4, height/2});

    Platform platform({0, height - 100}, {width, height - 100});
    Platform top({0, bigCircle.position.y}, {width, bigCircle.position.y});

    while (!WindowShouldClose())
    {
        BeginDrawing();

        logTexts(bigCircle, platform);
        DrawCircle(bigCircle.position.x, bigCircle.position.y, bigCircle.radius, WHITE);
        DrawLine(platform.startPos.x, platform.startPos.y, platform.endPos.x, platform.endPos.y, WHITE);
        DrawLine(top.startPos.x, top.startPos.y, top.endPos.x, top.endPos.y, WHITE);
        moveParticle(bigCircle, platform);
        ClearBackground(BLACK);
        EndDrawing();
    }
}