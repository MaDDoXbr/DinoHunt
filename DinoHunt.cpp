// DinoHunt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"
#include "DinoHunt.h"

//** C-style:
//typedef struct Animation {	// struct identifier
//	int first;
//	int last;
//	int cur;
//
//	float frame_duration;
//	float duration_left;
//
//	AnimationType type; //'Looping' assumed
//} Animation;			// typedef "Alias"
//=====
// Usage:
//struct _animation A = Animation();	<- Interchangeable
//Animation B = _animation();

Animation* Sprite::GetAnimationFromId(string id) {
	return this->animations[id];
}

Rectangle Sprite::getAnimationRect(Animation* self) {
	int column = self->cur % self->column_count;	// "inferred" current column
	int x_offset = 2 * column;
	//No offset in the first column //(self->cur % column_count == self->first ? 0 : x_offset)
	int x = (int)((self->cur % self->column_count) * 48.0f) + x_offset;	// 48 => sprite_width
	int y = (int)((self->cur / self->column_count) * 48.0f);
	return Rectangle{ (float)x, (float)y, 48.0f, 48.0f };
}

void Sprite::Draw() {
	this->size = Vector2(48.f, 48.f);
	//TODO: Calculate rectangle from this->pos
	DrawTexturePro(this->texture, this->getAnimationRect(this->curAnimation), 
				   Rectangle{ this->pos.x,this->pos.y,48,48 }, { 0.f, 0.f }, 0.f, WHITE);
	//DrawTexturePro(txtr_dinomom,	getAnimationRect(&anm_dinomom, 3), 
	//				Rectangle{ 80,96,48,48 }, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void Sprite::SetAnimation(string id, Animation* anim_p) {
	//this->animations.insert(pair(id, anim));	
	this->animations.emplace(id, anim_p);	//Slightly more efficient, no temporary "pair" construction in memory
	this->curAnimation = anim_p;
}

void Sprite::UpdateAnimations()
{
	for (pair pair : this->animations) {
		this->UpdateAnimation(pair.second);
	}
}

void Sprite::UpdateAnimation(string id)
{
	Sprite::UpdateAnimation(this->animations[id]);
}

void Sprite::UpdateAnimation(Animation* self) {
	float deltaTime = GetFrameTime();
	self->duration_left -= deltaTime;
	// show next frame
	if (self->duration_left <= 0.0f) {
		self->duration_left = self->frame_duration;
		self->cur++;
		// if it's the last frame
		if (self->cur >= self->last) //was _>_, leading to a last-frame-stuck bug
			self->cur = (self->type != ONESHOT) ? self->first : self->last;	// ternary/conditional operator
			//if (self->type == LOOPING)
			//	self->cur = self->first;
			//else
			//	self->cur = self->last;		//sticks to the last frame
	}
}

int main()
{
	const Vector2 screenSize = { 320, 240 };
	const float screenScaler = 1.f;		//TODO: Make all sprites read and apply it automatically
	Image img_bg = LoadImage("dino-game-bg.png");
	Image img_dinomom = LoadImage("dinomom-atlas_hq.png");

	InitWindow((int)(screenSize.x * screenScaler), (int)(screenSize.y * screenScaler), "Dino Hunt!");

	Texture txtr_bg = LoadTextureFromImage(img_bg);
	Texture txtr_dinomom = LoadTextureFromImage(img_dinomom);

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

	Vector2 v2one = Vector2{ 1.f, 1.f };
	Vector2 v2zero = Vector2{ 0.f, 0.f };

	Sprite spr_dinomom = Sprite(txtr_dinomom, Vector2{ 80.f,90.f });

	//Sprite(Texture _texture, Vector2 _pos = { 80.f, 90.f }, float _rot = { 0.f }, Vector2 _size = { 1.f,1.f },
	//	Vector2 _startPos = { 0.f, 0.f }, Vector2 _screenScale = { 1.f, 1.f },
	//	map<string, Animation*> _animations, Animation * _curAnimation) {

	Animation anm_dinomom = Animation{	.first = 0, .last = 6, .cur = 0,
										.frame_duration = 0.1f, .duration_left = 0.1f, 
										.type = LOOPING };

	spr_dinomom.SetAnimation("walk", &anm_dinomom);

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

		//animation_update(&anm_dinomom);
		spr_dinomom.UpdateAnimation("walk");

		BeginDrawing();
			ClearBackground(WHITE);
			DrawTexture(txtr_bg, 0, 0, WHITE);
			//DrawText("Texture loaded!", 160, 170, 10, DARKGREEN);
			// 
			//DrawTextureEx(txtr_dinomom, { 80.0f, 96.0f }, 0.0f, 1.0f, WHITE);
			// 
			//DrawTexturePro(	txtr_dinomom, 
			//				Rectangle{0,0,48,48}, Rectangle{80,96,48,48}, 
			//				{0.0f, 0.0f}, 0.0f, WHITE);
			//DrawTexturePro(	txtr_dinomom, 
			//				getAnimationRect(&anm_dinomom,3), Rectangle{80,96,48,48},
			//				{0.0f, 0.0f}, 0.0f, WHITE);
			spr_dinomom.Draw();

		EndDrawing();
	}

	CloseWindow();

	UnloadTexture(txtr_bg);

	UnloadImage(img_bg);
}

