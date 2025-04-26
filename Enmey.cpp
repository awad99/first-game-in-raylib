#include "enemy.h"

Enemy InitEnemy(int windowWidth, int windowHeight) {

    Enemy e = { 0 };
    e.texture = LoadTexture("Enemy/enemy.jpg");

    // Scale sprite to 10% of original size
    float scale = 0.1f;
    e.size = { e.texture.width * scale,
             e.texture.height * scale + 30};

    // Start inactive, set random respawn delay between 10 and 15 seconds
    e.active = false;
    e.respawnTimer = 0.0f;
    e.respawnDelay = (float)GetRandomValue(10, 15);

    // Default movement speed (can randomize later if desired)
    e.speed = 150.0f;

    return e;
}

void UpdateEnemy(Enemy* e) {
    float dt = GetFrameTime();

    if (e->active) {
        // Move left
        e->position.x -= e->speed * dt;

        // If off the left edge, deactivate and reset timer & delay
        if (e->position.x < -e->size.x) {
            e->active = false;
            e->respawnTimer = 0.0f;
            e->respawnDelay = (float)GetRandomValue(10, 15);
        }
    }
    else {
        // Count up until respawn
        e->respawnTimer += dt;
        if (e->respawnTimer >= e->respawnDelay) {
            // Spawn now at right edge
            int winW = GetScreenWidth();
            int winH = GetScreenHeight();
            e->position.x = (float)winW;

            // Random Y so sprite fully on screen
            e->position.y = (float)GetRandomValue(0, winH - (int)e->size.y);

            // Optionally randomize speed: e->speed = (float)GetRandomValue(100, 200);
            e->active = true;
        }
    }
}

void DrawEnemy(const Enemy* e) {
    if (!e->active) return;

    // Draw full texture into dst rect of size e->size at e->position
    Rectangle src = { 0, 0,
        (float)e->texture.width,
        (float)e->texture.height
    };
    Rectangle dst = { e->position.x,
                      e->position.y,
                      e->size.x,
                      e->size.y };
    DrawTexturePro(e->texture, src, dst, { 0, 0 }, 0.0f, WHITE);
}

void UnloadEnemy(Enemy* e) {
    UnloadTexture(e->texture);
}
