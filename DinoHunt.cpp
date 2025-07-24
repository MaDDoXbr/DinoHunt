// DinoHunt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

typedef struct Animation {
	int first;
	int last;
	int cur;

	float frame_duration;
	float duration_left;

	//AnimationType type; //'Looping' assumed
};

void animation_update(Animation* self) {
	float deltaTime = GetFrameTime();
	self->duration_left -= deltaTime;
	// show next frame
	if (self->duration_left <= 0.0f) {
		self->duration_left = self->frame_duration;
		self->cur++;
		// if it's the last frame
		if (self->cur > self->last)
			self->cur = self->first;	//for looping-type animations
	}
}

Rectangle animation_frame(Animation* self, int num_frames_per_row) {
	int x_offset = 2;
	int x = (int)((self->cur % num_frames_per_row) * 48.0f)+x_offset;	// 48 => sprite_width
	int y = (int)((self->cur / num_frames_per_row) * 48.0f);
	return Rectangle{ (float)x, (float)y, 48.0f, 48.0f };
}

int main()
{
	const Vector2 screenSize = { 320, 240 };
	Image img_bg = LoadImage("dino-game-bg.png");
	Image img_dinomom = LoadImage("dinomom-atlas_hq.png");

	InitWindow((int)screenSize.x, (int)screenSize.y, "Dino Hunt!");

	Texture txtr_bg = LoadTextureFromImage(img_bg);
	Texture txtr_dinomom = LoadTextureFromImage(img_dinomom);

	Animation anm_dinomom = Animation{ 0, 6, 0, 0.1, 0.1 };

	//--- Main Game Loop
	while (!WindowShouldClose()) {

		animation_update(&anm_dinomom);

		BeginDrawing();
			ClearBackground(WHITE);
			DrawTexture(txtr_bg, 0, 0, WHITE);
			DrawText("Texture loaded!", 160, 170, 10, DARKGREEN);
			//DrawTextureEx(txtr_dinomom, { 80.0f, 96.0f }, 0.0f, 1.0f, WHITE);
			// 
			//DrawTexturePro(	txtr_dinomom, 
			//				Rectangle{0,0,48,48}, Rectangle{80,96,48,48}, 
			//				{0.0f, 0.0f}, 0.0f, WHITE);
			DrawTexturePro(	txtr_dinomom, 
							animation_frame(&anm_dinomom,3), Rectangle{80,96,48,48},
							{0.0f, 0.0f}, 0.0f, WHITE);

		EndDrawing();
	}

	CloseWindow();

	UnloadTexture(txtr_bg);

	//UnloadImage(img_bg);
}
