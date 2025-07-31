#include "Sprite.h"

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
	int x_offset = this->cropOffset.x * column;	//TODO: add y-offset capacity
	//No offset in the first column //(self->cur % column_count == self->first ? 0 : x_offset)
	int x = (int)((self->cur % self->column_count) * this->size.x) + x_offset;	// 48 => sprite_width
	int y = (int)((self->cur / self->column_count) * this->size.y);
	return Rectangle{ (float)x, (float)y, this->size.x, this->size.y }; //was: 48x48
}

void Sprite::Draw() {
	//TODO: Calculate rectangle from this->pos
	DrawTexturePro(this->texture, this->getAnimationRect(this->curAnimation),
		Rectangle{ this->pos.x,this->pos.y,this->size.x,this->size.y }, { 0.f, 0.f }, 0.f, WHITE);	//TODO: Add "Speed"
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