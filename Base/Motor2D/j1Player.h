#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	//POWERUPS
	/*
	0-> boots
	1-> rifle
	2-> bullets
	*/

	int checkpoint = 0;

	int timer = 0;
	bool itstime;
	bool Start();
	bool Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* sprites = nullptr;
	Animation* current_animation = nullptr;

	fPoint position;

	Collider* col;
	bool destroyed = false;

	int font_score = -1;
	uint audio_shot;

	int score;
	char scores[8];

	int lifes = 3;

	bool joystick_up;
	bool joystick_down;
	bool joystick_left;
	bool joystick_right;

	bool death = false;

	int god_mode = 0;
};

#endif