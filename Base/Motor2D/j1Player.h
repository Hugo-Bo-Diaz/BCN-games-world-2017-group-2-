#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

struct player_char
{
	SDL_Texture* graphics = nullptr;
	SDL_Texture* sprites = nullptr;
	Animation* current_animation = nullptr;
	float jump_force;
	float speed;


	fPoint real_position;
	PhysBody* player;
	PhysBody* player_sliding;
	bool jumping = true;
	bool moving = false;
	bool sliding = false;
	bool face_right;
};


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

	void Jump(bool);

	void SlideStart();

	void SlideEnd();

	void GoRight(bool);

	void GoLeft(bool);

	void StopMoving(bool);

public:
	bool character_controll = true; //true para el gordete, false para la flaquita
	player_char characters[2];
};

#endif