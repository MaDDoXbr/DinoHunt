// DinoHunt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include "Sprite.h"

#define BG_SCROLL_SPEED 0.6
#define DINOMOM_SPEED 2.0
#define DINOBABY_SPEED 2.2
#define CAR_SPEED 2.8

float bgOffset = 0.0f;
vector<Sprite> AllSprites = {};
Sprite spr_dinomom;

const Vector2 screenSize = { 320, 240 };
const Rectangle dinomom_bounds = Rectangle{ .x = 10.f, .y = 20.f,
							.width = screenSize.x - 60.f - spr_dinomom.size.x,
							.height = screenSize.y - 70.f - spr_dinomom.size.y };

Animation anm_dinomom = Animation{ .first = 0, .last = 6, .cur = 0,
									.frame_duration = 0.1f, .duration_left = 0.1f,
									.type = LOOPING };
Animation anm_dinobaby = Animation{ .first = 0, .last = 6, .cur = 0,
									.frame_duration = 0.1f, .duration_left = 0.1f,
									.type = LOOPING };

int main()
{
	const float screenScaler = 1.f;		//TODO: Make all sprites read and apply it automatically
	Image img_bg = LoadImage("dino-game-bg.png");
	Image img_dinomom = LoadImage("dinomom-atlas_hq.png");
	Image img_dinobaby = LoadImage("babydino-atlas.png");
	Image img_car = LoadImage("car-atlas.png");

	InitWindow((int)(screenSize.x * screenScaler), (int)(screenSize.y * screenScaler), "Dino Hunt!");
	SetTargetFPS(60);

	Texture txtr_bg = LoadTextureFromImage(img_bg);
	Texture txtr_dinomom = LoadTextureFromImage(img_dinomom);
	Texture txtr_dinobaby = LoadTextureFromImage(img_dinobaby);
	Texture txtr_car = LoadTextureFromImage(img_car);

	spr_dinomom = Sprite(txtr_dinomom, Vector2{ 40.f,90.f }, Vector2{ 48.f, 48.f }, 0.f, Vector2{ 2.f, 0.f });
	spr_dinomom.SetAnimation("walk", &anm_dinomom);

	Sprite spr_dinobaby = Sprite(txtr_dinobaby, Vector2{ 200.f,90.f }, Vector2{ 34.f, 34.f });
	spr_dinobaby.SetAnimation("walk", &anm_dinobaby);

	//Animation anm_dinomom = Animation{ 0, 6, 0, 0.1, 0.1 };

	/// *** Pre-C++20 named initialization:
	//Animation anm_dinomom = [&] {
	//	Animation anm;
	//	anm.first = 0; anm.last = 6; anm.cur = 0; anm.frame_duration = 0.16; 
	//	anm.duration_left = 0.16;
	//	return anm;
	//	}();

	///	Named-parameters struct initialization requires C++20 setup (14 is default):
	//  1. Right-click Project -> Properties
	//  2. Configuration Properties -> C / C++ Language
	//	3. C++ Language Standard ->	ISO C++20 Standard(/ std:c++20)

	//using Animation = struct {
	//	Sprite* sprite;
	//	string id;
	//	Texture texture;
	//	int column_count = 3;	//rows are calculated from the column (TODO: add separate param for sparse spritesheets)
	//	int first; int last; int cur;
	//	float frame_duration; float duration_left;
	//	AnimationType type;
	//};

	//--- Main Game Loop
	while (!WindowShouldClose()) {

		spr_dinomom.UpdateAnimation("walk");
		spr_dinobaby.UpdateAnimation("walk");

		BeginDrawing();
			ClearBackground(WHITE);
			// Draw Scrolling BG
			bgOffset -= BG_SCROLL_SPEED;   //Scroll speed per screen update
			if (bgOffset <= -txtr_bg.width)
				bgOffset = 0;
			DrawTexture(txtr_bg, bgOffset, 0, WHITE);
			DrawTexture(txtr_bg, txtr_bg.width+bgOffset, 0, WHITE);

			// Player movement
			if (IsKeyDown(KEY_RIGHT))
				spr_dinomom.pos.x += DINOMOM_SPEED;
			if (IsKeyDown(KEY_LEFT))
				spr_dinomom.pos.x -= DINOMOM_SPEED;
			if (IsKeyDown(KEY_UP))
				spr_dinomom.pos.y -= DINOMOM_SPEED;
			if (IsKeyDown(KEY_DOWN))
				spr_dinomom.pos.y += DINOMOM_SPEED;
			spr_dinomom.pos.x = clamp(spr_dinomom.pos.x, dinomom_bounds.x, (dinomom_bounds.x + dinomom_bounds.width));
			spr_dinomom.pos.y = clamp(spr_dinomom.pos.y, dinomom_bounds.y, (dinomom_bounds.y + dinomom_bounds.height));

			spr_dinobaby.pos.x -= DINOBABY_SPEED;

			spr_dinobaby.Draw();
			spr_dinomom.Draw();

		EndDrawing();
	}

	//std::unique_ptr x; // only one pointer may own the resource at a time; released when unique o.o.scope
	//std::shared_ptr	y; // supports multiple owners, only released after all owners go out of scope
	//	// ^--- uses reference-counting
	//std::weak_ptr z;	// holds a non-owning reference to a shared_ptr; as long as there's one, no release
	//	// ^--- does not affect reference-counting, can NOT be dereferenced directly (requires lock() )
	//	// Used to "observe" objects existance, without causing circular references

	CloseWindow();

	UnloadTexture(txtr_bg);

	UnloadImage(img_bg);
}

//---------- MOCK CODE ARCHITECTURE BELOW:
//
//bool gameOver; bool isPaused;
//
//void GameSetup() {
//	gameOver = false;
//	spr_dinomom.pos = spr_dinomom.startPos;
//}
//
//void UpdateGame() {
//	if (!gameOver) {
//		if (IsKeyPressed('P'))
//			isPaused != isPaused;
//		if (!isPaused) {
//			PlayerMovement();
//			CheckCollisions();
//			SpawnCars();
//		}
//	} else {
//		if (IsKeyPressed(KEY_ENTER)) {
//			GameSetup();
//			gameOver = false;
//		}
//	}
//}
//
//void DrawGame() {
//	BeginDrawing();
//	ClearBackground(WHITE);
//	// Draw the scrolling Background
//	if (!gameOver) {
//		spr_dinomom.Draw();
//		for (Sprite thisSprite : AllSprites) {
//			thisSprite.Draw();
//		}
//		//DrawUI();
//	}
//	else {
//		DrawText("PRESS [ENTER] TO PLAY AGAIN", 30, 60, 40, WHITE);
//	}
//	EndDrawing();
//}
//
//void GameWrapup() {}
//
//int main_() {
//	InitWindow(400, 300, "Title");
//	SetTargetFPS(60);
//	GameSetup();	//Set starting values, 
//	while (!WindowShouldClose()) {
//		UpdateGame();
//		DrawGame();
//	}
//	GameWrapup();
//	CloseWindow();
//}

