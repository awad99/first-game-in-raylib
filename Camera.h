#pragma once
#include <raylib.h>

Camera2D InitCamera(int windowWidth, int windowHeight);

void resetCamera(Camera2D* camera);

void updateCamera(Camera2D* camera);