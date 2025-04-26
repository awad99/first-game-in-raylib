#include "enemy.h"
#include "Player.h"

Enemy InitEnemy(int windowWidth, int windowHeight) {
    Enemy e = { 0 };
    e.texture = LoadTexture("Enemy/enemy.jpg");

    float scale = 0.1f;
    e.size = { e.texture.width * scale,
               e.texture.height * scale + 30 };


    e.active = false;
    e.respawnTimer = 0.0f;
    e.respawnDelay = (float)GetRandomValue(5, 15);


    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        e.bullets[i].active = false;
    }
    e.speed = 150.0f;

    return e;
}

Enemy InitWitch(int windowWidth, int windowHeight) {
    Enemy w = { 0 };
    w.texture = LoadTexture("Enemy/enemy2.png");
    w.isWitch = true;
    w.speed = 200.0f;
    w.shootDelay = 0.8f;
 
    w.animFrames = 2;
    w.animInterval = 0.25f;
    w.currentFrame = 0;
    w.animTimer = 0.0f;
    float scale = 0.1f;
    w.size = {
      w.texture.width * scale,
      (w.texture.height / 2) * scale   
    };
    return w;
}

Enemy InitBull(int windowWidth, int windowHeight) {
    Enemy w = { 0 };
    w.texture = LoadTexture("Enemy/bull.png");
    w.isbull = true;
    w.speed = 250.0f;
    w.shootDelay = 0.8f;

    w.animFrames = 2;
    w.animInterval = 0.25f;
    w.currentFrame = 0;
    w.animTimer = 0.0f;
    float scale = 0.1f;
    w.size = {
      w.texture.width * scale,
      (w.texture.height / 2) * scale
    };
    return w;
}
void UpdateEnemy(Enemy* e, Player* p,Sound soundWitch, Sound soundBull, Sound soundShoot)  {
    float dt = GetFrameTime();

    float groundY = 355;
    float floatOffset = e->size.y + 30;
    int   respMin = e->isWitch ? 2 : 1;
    int   respMax = e->isWitch ? 5 : 3;


    if (e->active) {
        e->position.x -= e->speed * dt;

        if (e->position.x < -e->size.x) {
            e->active = false;
            e->respawnTimer = 0;
            e->respawnDelay = (float)GetRandomValue(respMin, respMax);
        }
    }
    else {
        e->respawnTimer += dt;
        if (e->respawnTimer >= e->respawnDelay) {
            e->position.x = GetScreenWidth();
            e->position.y = e->isWitch
                ? (GetScreenHeight() / 9) 
                : (e->isbull ?  groundY - 50 : groundY);
            e->active = true;
        }
    }

 
    e->shootTimer += dt;

    
    bool canShoot = !e->isWitch ||
        (fabsf(p->position.x - e->position.x) < 30.0f && p->position.y > e->position.y);

   
    if (e->isWitch) {
     
        if (canShoot) {
            if ((e->shootTimer >= e->shootDelay * 0.7f) ||
                (e->shootTimer <= e->shootDelay * 0.3f && e->shootTimer > 0)) {
                e->currentFrame = 1; 
            }
            else {
                e->currentFrame = 0; 
            }
        }
        else {
            e->currentFrame = 0; // Reset to normal when player not below
        }
    }

   
    if (e->shootTimer >= e->shootDelay && canShoot) {
        bool alreadyShooting = false;
        if (!e->isWitch && !e->isbull) {  // Add check for bull here
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                if (e->bullets[i].active) {
                    alreadyShooting = true;
                    break;
                }
            }
        }

        // This is the key part - only play sound when actually shooting
        if (e->isWitch || e->isbull || !alreadyShooting) {
            // Create bullet first
            bool bulletCreated = false;
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                Bullet* b = &e->bullets[i];
                if (!b->active) {
                    b->active = true;
                    b->position = {
                        e->position.x + e->size.x * 0.5f,
                        e->position.y + e->size.y * 0.5f
                    };
                    if (e->isWitch) {
                        b->velocity = { 0, 300.0f };
                    }
                    else {
                        b->velocity = { -300.0f, 0 };
                    }
                    b->radius = 5.0f;
                    bulletCreated = true;
                    break;
                }
            }

            // Only play sound if we actually created a bullet
            if (bulletCreated) {
                if (e->isWitch) {
                    PlaySound(soundWitch);
                }
                else if (e->isbull) {
                    PlaySound(soundBull);
                }
                else {
                  //  PlaySound(soundShoot);
                }
            }

            e->shootTimer = 0;
        }
    } {
       
        bool alreadyShooting = false;
        if (!e->isWitch) {
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                if (e->bullets[i].active) {
                    alreadyShooting = true;
                    break;
                }
            }
        }

        if (e->isWitch || !alreadyShooting) {
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                Bullet* b = &e->bullets[i];
                if (!b->active) {
                    b->active = true;
                    b->position = {
                        e->position.x + e->size.x * 0.5f,
                        e->position.y + e->size.y * 0.5f
                    };
                    if (e->isWitch) {
                        b->velocity = { 0, 300.0f };
                    }
                    else {
                        b->velocity = { -300.0f, 0 };
                    }
                    b->radius = 5.0f;
                    break;
                }
            }


            e->shootTimer = 0;
        }
    }
    if (e->active && e->isbull)
    {
        e->animTimer += dt;
        if (e->animTimer >= e->animInterval) {
            e->animTimer -= e->animInterval;
            e->currentFrame = (e->currentFrame + 1) % e->animFrames;
        }
    }
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        Bullet* b = &e->bullets[i];
        if (!b->active) continue;
        b->position.x += b->velocity.x * dt;
        b->position.y += b->velocity.y * dt;
        Rectangle pr = {
            p->position.x, p->position.y,
            p->size.x,     p->size.y
        };
        if (CheckCollisionCircleRec(b->position, b->radius, pr)) {
            if (e->isWitch) {
                p->slowTimer = 5.0f;
                p->speed *= 0.5f;
            }
            else
            p->Isactive = false;

            b->active = false;
        }
        if (b->position.x < -b->radius
            || b->position.y > GetScreenHeight() + b->radius)
            b->active = false;
    }
}

void DrawEnemy(Enemy* e) {
    if (!e->active) return;

    Rectangle src = {
        0, 0,
        (float)e->texture.width,
        (float)e->texture.height
    };

    if (e->isWitch) {
        float frameHeight = e->texture.height / e->animFrames;
        src.y = e->currentFrame * frameHeight;
        src.height = frameHeight;
    }

    if (e->isbull) {
        float frameHeight = e->texture.height / e->animFrames;
        src.y = e->currentFrame * frameHeight;
        src.height = frameHeight;
    }
    Rectangle dst = {
        e->position.x,
        e->position.y,
        e->size.x,
        e->size.y
    };

    DrawTexturePro(e->texture, src, dst, { 0, 0 }, 0.0f, WHITE);

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        Bullet* b = &e->bullets[i];
        if (!b->active) continue;
        Color col = e->isWitch ? BLACK : RED;
        DrawCircle(b->position.x, b->position.y, b->radius, col);
    }
}