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
	//Sprite* sprite;
	string id;
	Texture texture;
	int column_count = 3;	//rows are calculated from the column (TODO: add separate param for sparse spritesheets)
	int first; int last; int cur;
	float frame_duration; float duration_left;
	AnimationType type;
};

//**********************

class Sprite {
public:
	Texture texture{};
	Vector2 size{ 1.f, 1.f };		// in pixels
	Vector2 screenScale{ 1.f, 1.f };// on-screen
	Vector2 startPos{ 0.f, 0.f };
	Vector2 pos{ 80.f, 90.f };	//0f, 0f
	Vector2 cropOffset{ 0.f, 0.f };
	float rot{ 0.f };
	map<string, Animation*> animations;	// animname | animation
	Animation* curAnimation;

	Sprite(Texture _texture,
		Vector2 _pos = { 80.f, 90.f },
		Vector2 _size = { 1.f,1.f },
		float _rot = { 0.f },
		Vector2 _cropOffset = { 0.f, 0.f },
		Vector2 _screenScale = { 1.f, 1.f },
		Vector2 _startPos = { 0.f, 0.f },
		map<string, Animation*> _animations = {},
		Animation* _curAnimation = nullptr) {

		texture = _texture;	pos = _pos;	rot = _rot; size = _size;
		startPos = _startPos; screenScale = _screenScale; cropOffset = _cropOffset;
		animations = _animations; curAnimation = _curAnimation;
	}

	Sprite() {};

	//~Sprite() { cout << "Sprite destroyed!"; } //Destructor

	void Draw();
	void SetAnimation(string, Animation*);	//Might be "add" or "update"
	Animation* GetAnimationFromId(string);
	void UpdateAnimations();
	void UpdateAnimation(string);			//Overload with Id (string)
	void UpdateAnimation(Animation*);		//Overload with the actual Animation pointer
	Rectangle getAnimationRect(Animation*);

private:


};