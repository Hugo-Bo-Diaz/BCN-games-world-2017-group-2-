#include "j1App.h"
#include "Enemy_1.h"
#include "j1Collision.h"
#include "j1Enemies.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "j1Player.h"
#include "SDL/include/SDL_timer.h"
#include "j1Particles.h"

#define PATH_DURATION 500
#define BULLET_INT_MIN 700
#define BULLET_INT_MAX 1000


Enemy_1::Enemy_1(int x, int y) : Enemy(x, y)
{

	//gris esnemy_browncookie
	animation.PushBack({ 145, 83, 16, 28 });
	animation.PushBack({ 164, 83, 18, 28 });
	animation.PushBack({ 185, 83, 20, 28 });
	animation.speed = 0.1f;

	animation = animation;

	srand(time(NULL));

	collider = App->collision->AddCollider({0, 0, 18, 27}, COLLIDER_TYPE::COLLIDER_ENEMY, (Module*)App->enemies);
	col = App->collision->AddCollider({ 0, 0, 18, 10 }, COLLIDER_TYPE::COLLIDER_ENEMY_BASE,(Module*)App->enemies);

	original_pos.y = y;
	path_dest.y = position.y;
	path_dest.x = position.x;
	hp = 1;
	
}

int Enemy_1::value_between(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void Enemy_1::Move()
{
	col->SetPos(position.x, position.y + 17);
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

}
Enemy_1::~Enemy_1()
{
	if (App->player->destroyed == false && hit == true)
	{
		App->player->score += 200;
		App->enemies->Playsound(1);
	}
	App->particles->AddParticle(App->particles->cookiedeath, position.x, position.y, COLLIDER_NONE);
}