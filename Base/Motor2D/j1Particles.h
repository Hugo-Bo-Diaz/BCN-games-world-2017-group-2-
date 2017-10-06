#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "j1Module.h"
#include "Animation.h"
#include "P2Defs.h"
#include "p2Point.h"
//#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 500

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Update();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	uint tnt_sound;

	//void AddParticle(const Particle& particle, int x, int y, Uint32 delay = 0, int xspeed = 100, int yspeed = 100);

private:

	SDL_Texture* graphics = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];

public:

	bool powerup_activated = false;

	Particle explosion;
	Particle laser;
	Particle shot_l;
	Particle shot_r;
	Particle shot_g;
};

#endif // __MODULEPARTICLES_H__