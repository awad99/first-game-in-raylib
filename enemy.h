#pragma once
#include <raylib.h>
#include <iostream>
#include "Player.h"
#define MAX_ENEMY_BULLETS 10

typedef struct {
    Vector2 position;
    float radius;
    bool active;
    float speed;
    Vector2 velocity;
}Bullet;

typedef struct {
    Vector2   position;     
    Vector2   size;         
    Texture2D texture;    
    Texture2D witchtexturxe;
    Texture2D bulltexture;
    float     speed;        
    bool      active;        
    float     respawnTimer; 
    float     respawnDelay; 

    Bullet bullets[MAX_ENEMY_BULLETS];
    float shootTimer;
    float shootDelay;
    bool isWitch;
    bool isbull;
    int animFrames;
    int currentFrame;
    float animTimer;
    float animInterval;
} Enemy;

Enemy InitEnemy(int windowWidth, int windowHeight);
Enemy InitWitch(int windowWidth, int windowHeight);
Enemy InitBull(int windowWidth, int windowHeight);
void UpdateEnemy(Enemy* e, Player* p, Sound soundWitch, Sound soundBull, Sound soundShoot);
void DrawEnemy(Enemy* enemy);