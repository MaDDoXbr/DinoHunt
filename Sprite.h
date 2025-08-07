#pragma once

using namespace std;
#include <string>
#include <map>
#include "raylib.h"

//** C-style:
//typedef enum AnimationType {
//	LOOPING = 1,
//	ONESHOT = 2,
//} AnimationType;

//** C++-style:
using AnimationType = enum { LOOPING = 1, ONESHOT = 2, };

//******************************

struct Animation {
	//Texture texture;
	Rectangle sourceRect = {};	// Area that'll be sliced from the spritesheet
	int column_count = 3;	//rows are calculated from the column (TODO: add separate param for sparse spritesheets)
	int first; int last; int cur;
	float frame_duration; float duration_left;
	AnimationType type;
	Vector2 cropOffset{ 0.f, 0.f };
};

//**********************

class Sprite {
public:
	Texture texture{};
	Vector2 screenScale{ 1.f, 1.f };// on-screen
	Rectangle rect;
	float rot{ 0.f };
	map<string, Animation*> animations;	// animname | animation
	Animation* curAnimation;
	bool active = true;

	Sprite(Texture texture,
		Rectangle rect = {0.f, 0.f, 0.f, 0.f}, //{ .x = 80.f, .y = 90.f, .width = 1.f, .height = 1.f },
		float rot = { 0.f },
		bool active = true,
		Vector2 screenScale = { 1.f, 1.f },
		map<string, Animation*> animations = {},
		Animation* curAnimation = nullptr) 
		{
			this->texture = texture; this->rect = rect; this->rot = rot;
			this->screenScale = screenScale;
			this->animations = animations; this->curAnimation = curAnimation;
		}
	// Default constructor with member initializers to prevent "uninitialized" error messages
	Sprite() : rect{0.f, 0.f, 0.f, 0.f}, curAnimation(nullptr) {}
	//~Sprite() { cout << "Sprite destroyed!"; } //Destructor

	void Draw();
	void SetAnimation(string, Animation*);	//Might be "add" or "update"
	Animation* GetAnimationFromId(string);
	void UpdateAnimations();
	void UpdateAnimation();
	void UpdateAnimation(string);			//Overload with Id (string)
	void UpdateAnimation(Animation*);		//Overload with the actual Animation pointer
	void updateAnimFrame(Animation*);

private:


};