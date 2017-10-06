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

	int checkpoint = 0;

	int timer = 0;
	bool itstime;

	bool Awake(const pugi::xml_node& config);
	bool Start();

	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody*, PhysBody*);

	bool Load(const pugi::xml_node& config);
	bool Save(const pugi::xml_node& config);


public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* sprites = nullptr;
	Animation* current_animation = nullptr;

	
	Animation idle;
	Animation left;
	Animation right;
	Animation special_1;
	Animation special_2;
	Animation special_3;

	fPoint position; // en desuso
	Collider* col; // en desuso
	float jump_force;

	int font_score = -1;
	uint audio_shot;

	int score;
	char scores[8];

	int lifes = 3;

	bool joystick_up;
	bool joystick_down;
	bool joystick_left;
	bool joystick_right;

	float joystick_angle;

	bool death = false;

	float speed;
	int god_mode = 0;



	PhysBody* player;
    bool jumping = true;
    bool moving = false;

	



};

#endif