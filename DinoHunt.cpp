// DinoHunt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include "Sprite.h"

#define BG_SCROLL_SPEED 0.6f
#define DINOMOM_SPEED 2.0f
#define DINOBABY_SPEED 2.2f
#define CAR_SPEED 2.8f

#define NUM_MAX_DINOBABIES 50

Sprite spr_babydinos[NUM_MAX_DINOBABIES]; // = { Sprite{} };

float bgOffset = 0.0f;
vector<Sprite*> AllSprites = {};
Sprite spr_dinomom;

//Game Variables
int Score = 0;

const Vector2 screenSize = { 320, 240 };
const Rectangle dinomom_bounds = Rectangle{ .x = 10.f, .y = 20.f,
							.width = screenSize.x - 60.f - 48.f,
							.height = screenSize.y - 70.f - 48.f };

//struct Animation {
//	string id;
//	Texture texture;
//	int column_count = 3;	//rows are calculated from the column (TODO: add separate param for sparse spritesheets)
//	Rectangle sourceRect = {};	// Area that'll be sliced from the spritesheet// 
//	int first; int last; int cur;
//	float frame_duration; float duration_left;
//	AnimationType type;
//};

Animation anm_dinomom = Animation{	.sourceRect = Rectangle{ 0.f, 0.f, 48.f, 48.f },
									.first = 0, .last = 6, .cur = 0,
									.frame_duration = 0.1f, .duration_left = 0.1f,
									.type = LOOPING, .cropOffset = { 2.f, 0.f } };
Animation anm_dinobaby = Animation{ .sourceRect = Rectangle{ 0.f, 0.f, 34.f, 34.f },
									.first = 0, .last = 6, .cur = 0,
									.frame_duration = 0.1f, .duration_left = 0.1f,
									.type = LOOPING };

const float screenScaler = 1.f;		//TODO: Make all sprites read and apply it automatically
Image img_bg = LoadImage("dino-game-bg.png");
Image img_dinomom = LoadImage("dinomom-atlas_hq.png");
Image img_dinobaby = LoadImage("babydino-atlas.png");
Image img_car = LoadImage("car-atlas.png");

int main()
{

	InitWindow((int)(screenSize.x * screenScaler), (int)(screenSize.y * screenScaler), "Dino Hunt!");
	SetTargetFPS(60);

	Texture txtr_bg = LoadTextureFromImage(img_bg);
	Texture txtr_dinomom = LoadTextureFromImage(img_dinomom);
	Texture txtr_dinobaby = LoadTextureFromImage(img_dinobaby);
	Texture txtr_car = LoadTextureFromImage(img_car);

	//Dinomom sprite
	spr_dinomom = Sprite(	txtr_dinomom, Rectangle{ .x=40.f, .y=90.f, .width=48.f, .height=48.f }, 0.f	);
	spr_dinomom.SetAnimation("walk", &anm_dinomom);
	AllSprites.push_back(&spr_dinomom);

	//Dinobaby sprite
	//Dynamic Memory allocation (use "delete" to destroy it later)
	Sprite* spr_dinobaby = new Sprite(txtr_dinobaby, { .x = 200.f, .y = 90.f, .width = 34.f, .height = 34.f });
	spr_dinobaby->SetAnimation("walk", &anm_dinobaby);
	AllSprites.push_back(spr_dinobaby);

	//--- Main Game Loop
	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(WHITE);
			// Draw Scrolling BG
			bgOffset -= BG_SCROLL_SPEED;   //Scroll speed per screen update
			if (bgOffset <= -txtr_bg.width)
				bgOffset = 0;
			DrawTexture(txtr_bg, (int)bgOffset, 0, WHITE);
			DrawTexture(txtr_bg, (int)(txtr_bg.width + bgOffset), 0, WHITE);

			// Player movement
			if (IsKeyDown(KEY_RIGHT))
				spr_dinomom.rect.x += DINOMOM_SPEED;
			if (IsKeyDown(KEY_LEFT))
				spr_dinomom.rect.x -= DINOMOM_SPEED;
			if (IsKeyDown(KEY_UP))
				spr_dinomom.rect.y -= DINOMOM_SPEED;
			if (IsKeyDown(KEY_DOWN))
				spr_dinomom.rect.y += DINOMOM_SPEED;
			spr_dinomom.rect.x = clamp(spr_dinomom.rect.x, dinomom_bounds.x, (dinomom_bounds.x + dinomom_bounds.width));
			spr_dinomom.rect.y = clamp(spr_dinomom.rect.y, dinomom_bounds.y, (dinomom_bounds.y + dinomom_bounds.height));

			//for (int i = 0; i < NUM_MAX_BABYDINOS; i++) {
			//	spr_dinobabies[i]
			//}

			if (spr_dinobaby->active) {
				//Move Dinobabies
				spr_dinobaby->rect.x -= DINOBABY_SPEED;
				//Check Collisions
				if (CheckCollisionRecs(spr_dinomom.rect, spr_dinobaby->rect))
				{
					Score++;
					spr_dinobaby->active = false;
					//delete spr_dinobaby;	// free up memory that was dynamically allocated
					//spr_dinobaby = nullptr; //creates a dangling pointer in AllSprites
				}
			}

			DrawTextEx(GetFontDefault(), TextFormat("Score: %d", Score), Vector2{ (screenSize.x - 80), 10 }, 18, 2, RED);

			//spr_dinobaby.Draw();
			//spr_dinomom.Draw();
			for (Sprite* sprite : AllSprites) {
				if (sprite->active)
					sprite->Draw();
			}

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

