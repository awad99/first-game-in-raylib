#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "enemy.h"

typedef enum { INTRO,PLAY,GameOver} GameState;

void RestGame(Enemy* enemy, Enemy* witch, Player* player, int windowWidth, int windowHeight)
{
	*player = InitPlayer(windowWidth, windowHeight);
	*enemy = InitEnemy(windowWidth, windowHeight);
	*witch = InitWitch(windowWidth, windowHeight);
}
int main()
{
	InitAudioDevice();
	int windowWidth = 750;
	int windowHight = 700;
	InitWindow(windowWidth, windowHight, "my game");
	Player player = InitPlayer(windowWidth, windowHight);
	Map map = InitMap(windowWidth, windowHight);
	Camera2D camera = InitCamera(windowWidth, windowHight);
	Enemy enemy = InitEnemy(windowWidth, windowHight);
	Enemy witch = InitWitch(windowWidth, windowHight);
	Enemy bull = InitBull(windowWidth, windowHight);

	Sound soundwitch = LoadSound("Sound/witch.wav");
	Sound soundbull = LoadSound("Sound/bull.wav");
	Sound soundshoot = LoadSound("Sound/shoot.wav");

	GameState gamestate = INTRO;

	const char* message = "Press ENTER to start game";
	const char* messagegameover = "GAME OVER press Enter to play";
	int fontSize = 20;
	int textWidth = MeasureText(message, fontSize);
	int textx = (windowWidth - textWidth) / 2;
	int texty = windowHight / 2;


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(SKYBLUE);

		switch (gamestate)
		{
		case INTRO:
			if (IsKeyPressed(KEY_ENTER))
				gamestate = PLAY;
			DrawText(message, textx, texty, fontSize, BLACK);
			drawMap(&map);
			drawPlayer(&player);
			break;
		case PLAY:
			updateplayer(&player); 
			updateMap(&map);
			drawMap(&map);
			DrawEnemy(&enemy);
			DrawEnemy(&witch);
			DrawEnemy(&bull);
			UpdateEnemy(&enemy,&player,soundwitch,soundbull,soundshoot);
			UpdateEnemy(&witch, &player,soundwitch,soundbull,soundshoot);
			UpdateEnemy(&bull, &player,soundwitch,soundbull,soundshoot);
			drawPlayer(&player);

			if (!player.Isactive)
				gamestate = GameOver;
			break;
		case GameOver:
			DrawText(messagegameover, textx, texty, fontSize, RED);
			drawMap(&map);
			drawPlayer(&player);
			DrawEnemy(&enemy);

			if (IsKeyPressed(KEY_ENTER)) {
				RestGame(&enemy, &witch, &player, windowWidth, windowHight);
				gamestate = PLAY;
			}
		}
		
		EndDrawing();
	}

	UnloadTexture(map.blockTexture);
	UnloadTexture(map.block2Texture);
	UnloadTexture(map.cloudTexture);
	UnloadTexture(map.TreeTexture);
	UnloadTexture(player.playerTexture);
	UnloadTexture(enemy.texture);
	CloseWindow();
	CloseAudioDevice();
}