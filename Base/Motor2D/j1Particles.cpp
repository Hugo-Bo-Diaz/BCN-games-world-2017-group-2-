#include <math.h>
#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Render.h"
//#include "j1Collision.h"
#include "j1Particles.h"
#include "j1Player.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;

	explosion.anim.PushBack({ 22, 46, 31, 31 });
	explosion.anim.PushBack({ 55, 46, 31, 31 });
	explosion.anim.PushBack({ 94, 46, 31, 31 });
	explosion.anim.PushBack({ 134, 46, 31, 31 });
	explosion.anim.PushBack({ 175, 46, 31, 31 });
	explosion.anim.PushBack({ 214, 45, 31, 31 });
	explosion.anim.PushBack({ 254, 46, 31, 31 });
	explosion.anim.PushBack({ 297, 46, 31, 31 });
	explosion.anim.PushBack({ 333, 43, 31, 31 }); //333 43 = +3x-100y
	explosion.anim.PushBack({ 373, 46, 31, 31 });	
	explosion.anim.loop = false;
	explosion.anim.speed = 0.15f;

	laser.anim.PushBack({ 131, 37, 2, 4 });
	laser.anim.PushBack({339, 131, 5, 6 });
	laser.anim.PushBack({361, 129, 11, 10});
	laser.anim.speed = 0.0f;
	laser.speed.y = -4;
	laser.life = 750;

	//fixed up this point
	shot_l.anim.PushBack({ 174, 38, 3, 3 });
	shot_l.anim.PushBack({ 339, 131, 5, 6 });
	shot_l.anim.PushBack({ 361, 129, 11, 10 });
	shot_l.anim.speed = 0.0f;
	shot_l.speed.y = -3;
	shot_l.speed.x = -3;
	shot_l.life = 750;

	shot_r.anim.PushBack({ 178, 38, 3, 3 });
	shot_r.anim.PushBack({ 339, 131, 5, 6 });
	shot_r.anim.PushBack({ 361, 129, 11, 10 });
	shot_r.anim.speed = 0.0f;
	shot_r.speed.y = -3;
	shot_r.speed.x = +3;
	shot_r.life = 750;


	shot_g.anim.PushBack({ 316, 12, 10, 10 });
	shot_g.anim.speed = 0.0f;
	shot_g.speed.y = +5;
	shot_g.life = 750;    


}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");
	graphics = App->tex->Load("gunsmoke/particles.png");
	tnt_sound = App->audio->LoadFx("gunsmoke/tnt_explosion.wav");
	// Load particles fx particle

	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles /n");
//	App->tex->Unload(graphics);

	// Unload fx

	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
bool j1Particles::Update()
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if(p == nullptr)
			continue;

		if(p->Update() == false)
		{
			delete p;
			//p = nullptr;    //WE CHANGED THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			active[i] = nullptr;
		}
		else if(SDL_GetTicks() >= p->born)
		{
			App->render->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
			if(p->fx_played == false)
			{
				p->fx_played = true;
			}
		}
	}

	return true;
}

/*void j1Particles::AddParticle(const Particle& particle, int x, int y, Uint32 delay, int xspeed, int yspeed)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			if (xspeed != 100)
			{			
				p->speed.x = xspeed;
			}
			if (yspeed != 100)
			{
				p->speed.y = yspeed;
			}			
			/*if(collider_type != COLLIDER_NONE)
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
			active[i] = p;

			break;
		}
	}
}
*/
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if(active[i] != nullptr && active[i]->collider == c1)
		{
			//App->particles->AddParticle(App->particles->hitmarker, active[i]->position.x - 4, active[i]->position.y-4, COLLIDER_PARTICLE, 0);
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : 
anim(p.anim), position(p.position), speed(p.speed),
fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{

	/*if (collider != nullptr)
		collider->to_delete = true;*/
}

bool Particle::Update()
{
	bool ret = true;

	if(life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
		{
			/*if (collider != nullptr && collider->type == COLLIDER_PLAYER_SHOT)
			{
				App->particles->AddParticle(App->particles->end_of_bullet, position.x-4, position.y-4, COLLIDER_PARTICLE, 0);
			}
			*/
			ret = false;
		}
	}
	else
		if(anim.Finished())
			ret = false;

	position.x += speed.x;
	position.y += speed.y;
	/*
	if(collider != nullptr)
		collider->SetPos(position.x, position.y);
		*/
	return ret;
}

