// DinoHunt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

int main()
{
	const Vector2 screenSize = { 320, 240 };
	Image img = LoadImage("dino-game-bg.png");

	InitWindow((int)screenSize.x, (int)screenSize.y, "Dino Hunt!");


	//Texture2D texture = LoadTexture("dino-game-bg.png");
	Texture texture = LoadTextureFromImage(img);

	//--- Main Game Loop
	while (!WindowShouldClose()) {
		ClearBackground(WHITE);
		BeginDrawing();
			DrawTexture(texture, 0, 0, WHITE);
			DrawText("Texture loaded!", 160, 170, 10, DARKGREEN);
			//DrawTextureEx(texture, { 0.0f, 0.0f }, 0.0f, 0.0f, WHITE);
		EndDrawing();
	}

	CloseWindow();

	UnloadTexture(texture);

	//UnloadImage(img);
}
