#include "Camera.h"
#include "Player.h"

Camera2D InitCamera(int windowWidth, int windowHeight) {
	Camera2D camera = { 0 };
	camera.target = { 0.0f,0.0f };
	camera.offset = { 0.0f,0.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return camera;
}

void resetCamera(Camera2D* camera)
{
	camera->target = { 0.0f,0.0f };
}

void updateCamera(Camera2D* camera, Player* player)
{
	camera->target = {
		player->position.x + player->size.x * 1.5f,
	};
}