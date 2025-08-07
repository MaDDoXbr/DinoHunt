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

//Updates the sourceRectangle (in the animation) with the proper rectangle area
void Sprite::updateAnimFrame(Animation* anm) {
	int column = anm->cur % anm->column_count;	// "inferred" current column
	int x_offset = (int)(anm->cropOffset.x * column);	//TODO: add y-offset capacity
	//No offset in the first column //(anm->cur % column_count == anm->first ? 0 : x_offset)
	int x = (int)((anm->cur % anm->column_count) * anm->sourceRect.width) + x_offset;	// 48 => sprite_width
	int y = (int)((anm->cur / anm->column_count) * anm->sourceRect.height);
	//anm->sourceRect = Rectangle{ (float)x, (float)y, anm->sourceRect.width, anm->sourceRect.height }; //was: 48x48
	anm->sourceRect.x = (float)x;
	anm->sourceRect.y = (float)y;
}

void Sprite::Draw() {
	//Checks for looping
	float deltaTime = GetFrameTime();	//TODO: Get global time param, with a switch
	Animation* curAnim = this->curAnimation;
	curAnim->duration_left -= deltaTime;
	// show next frame
	if (curAnim->duration_left <= 0.0f) {
		curAnim->duration_left = curAnim->frame_duration;
		curAnim->cur++;
		// if it's the last frame
		if (curAnim->cur >= curAnim->last)
			curAnim->cur = (curAnim->type != ONESHOT) ? curAnim->first : curAnim->last;	// ternary/conditional operator
	}
	updateAnimFrame(curAnim);	//TODO: Check if passing curAnim is needed; probably not
	DrawTexturePro(	this->texture, curAnim->sourceRect,
					this->rect, { 0.f, 0.f }, 0.f, WHITE);
	//DrawTexturePro(txtr_dinomom,	updateAnimFrame(&anm_dinomom, 3), 
	//				Rectangle{ 80,96,48,48 }, { 0.0f, 0.0f }, 0.0f, WHITE);
}

/// <summary>Adds or updates one animation</summary>
/// <param name="id"></param>
/// <param name="anim_p">Animation Pointer</param>
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

/// <summary>Updates the currently selected animation</summary>
void Sprite::UpdateAnimation()
{
	Sprite::UpdateAnimation(this->curAnimation);
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
		if (self->cur >= self->last)
			self->cur = (self->type != ONESHOT) ? self->first : self->last;	// ternary/conditional operator
	}
}