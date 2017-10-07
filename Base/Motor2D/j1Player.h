#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

struct player_char
{
	SDL_Texture* graphics;
	//SDL_Texture* sprites = nullptr;
	Animation* current_animation = nullptr;
	p2SString animation_to_blit;
	p2List<Animation*>	animations;
	float jump_force;
	float speed;

	iPoint offset;
	float render_scale;

	fPoint real_position;
	PhysBody* player;
	PhysBody* player_sliding;
	PhysBody* player_anchor;
	bool jumping = true;
	bool moving = false;
	bool sliding = false;
	bool anchored = false;
	bool face_right = false;
	bool face_left = false;

	Animation* FindAnimByName(p2SString _name_) {
		p2List_item<Animation*>* ret = animations.start;
		while (ret->data->name != _name_) {
			ret = ret->next;
		}

		return ret->data;
	}
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


	bool LoadSprites(const pugi::xml_node& sprite_node);
	bool LoadProperties(const pugi::xml_node& property_node);
public:

	void Jump(bool);

	void SlideStart();

	void SlideEnd();

	void GoRight(bool);

	void GoLeft(bool);


	void StopMoving(bool);

	void AnchorStart();

	void AnchorEnd();

public:
	bool character_controll = true; //true para el gordete, false para la flaquita
	player_char characters[2];
	pugi::xml_node Local_config;
};

#endif