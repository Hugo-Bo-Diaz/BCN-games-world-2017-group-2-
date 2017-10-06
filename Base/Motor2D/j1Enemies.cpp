#include "P2Defs.h"
#include "P2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Enemies.h"
#include "j1Particles.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "Enemy.h"
#include "Enemy_1.h"

#define SPAWN_MARGIN 650

j1Enemies::j1Enemies()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->tex->Load("gunsmoke/enemies.png");
	sounds[0] = App->audio->LoadFx("gunsmoke/bomber_rifleman_death.wav");
	sounds[1] = App->audio->LoadFx("gunsmoke/bandit_sniper_death.wav");
	
	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].y * App->win->scale > App->render->camera.y + (App->render->camera.h * App->win->scale) - SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y * App->win->scale);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Enemies::Update()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) 
		{ 
			enemies[i]->previous = enemies[i]->position;
			enemies[i]->Move();
		};

	for(uint i = 0; i < MAX_ENEMIES; ++i)
		if(enemies[i] != nullptr) enemies[i]->Draw(sprites);
	if (boss_alive)
	{
		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (enemies[i] != nullptr && enemies[i]->boss == true)
			{
				int squares = enemies[i]->squares;
				for (int i = 0; i < squares; ++i)
				{
					App->render->Blit(sprites, 5 + 25 * i, -2795, &section);
				}
			}
		}
	}

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to de-spawn
	/*for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			if (enemies[i]->position.y * App->win->scale >(App->render->camera.y) + 495 || enemies[i]->position.y < -3000
				|| (App->player->destroyed == false ))
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.y * App->win->scale);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}*/

	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	/*App->tex->Unload(sprites);
	App->audio->UnLoadFx(sounds[0]);
	App->audio->UnLoadFx(sounds[1]);*/

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			{
				queue[i].type = ENEMY_TYPES::NO_TYPE;
			}
		}
	}


	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for(; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if(i != MAX_ENEMIES)
	{
		switch(info.type)
		{
			case ENEMY_TYPES::FIRST:
			enemies[i] = new Enemy_1(info.x,info.y);
			break;
		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if ((enemies[i] != nullptr) && ((enemies[i]->GetCollider() == c1) || (enemies[i]->GetCol() == c1)))
		{
			enemies[i]->OnCollision(c1, c2);
			/*if (c2->type == COLLIDER_PLAYER_SHOT)
			{
				enemies[i]->hp -= 1;
				enemies[i]->hit = true;
				if (enemies[i]->hp == 0)
				{
					delete enemies[i];
					enemies[i] = nullptr;
				}
			}*/
			break;
		}
	}
}

int j1Enemies::OnScreenEnemies()
{
	int ret = 0;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			ret++;
		}
	}
	return ret;
}

void j1Enemies::Playsound(int sound)
{
	App->audio->PlayFx(sounds[sound]);
}