#include "p2defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Particles.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Fonts.h"
#include "j1Audio.h"
#include "j1Enemies.h"
#include "SDL/include/SDL_timer.h"

#include<stdio.h>

j1Player::j1Player()
{

	// idle animation
	idle.PushBack({ 4, 8, 19, 29 });
	idle.PushBack({ 25, 8, 20, 29 });
	idle.PushBack({ 48, 8, 19, 29 });
	idle.PushBack({ 70, 8, 20, 29 });
	idle.PushBack({ 91, 8, 21, 29 });
	idle.speed = 0.2f;
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("LOAD ME HERE.png");

	destroyed = false;
	position.x = 0;//READ HERE FROM XML
	position.y = 0;//READ HERE FROM XML
	
	audio_shot = App->audio->LoadFx("gunsmoke/shotfx.wav");

	font_score = App->fonts->Load("fonts/font.png", "0123456789abcdefghijklmnopqrstuvwxyz", 1);
	col = App->collision->AddCollider({(int)position.x, (int)position.y, 19, 28}, COLLIDER_PLAYER, this);

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->audio->Unload(audio_shot);
	App->tex->Unload(graphics);
	App->fonts->UnLoad(font_score);

	if (col != nullptr)
		col->to_delete = true;

	return true;
}

// Update: draw background
bool j1Player::Update()
{

	float speed = 1 + 0.12;

	joystick_left = 0;
	joystick_right = 0;



		if (App->input->controller_1.left_joystick.x > 0.25)
		{
			joystick_right = 1;
		}
		if (App->input->controller_1.left_joystick.x < -0.25)
		{
			joystick_left = 1;
		}

		joystick_down = 0;
		joystick_up = 0;

		if (App->input->controller_1.left_joystick.y > 0.25)
		{
			joystick_down = 1;
		}
		if (App->input->controller_1.left_joystick.y < -0.25)
		{
			joystick_up = 1;
		}


		if ((App->input->GetKey[SDL_SCANCODE_W] == KEY_REPEAT || joystick_up || App->input->controller_1.w_button))
		{
			position.y -= speed;
		}

		if ((App->input->GetKey[SDL_SCANCODE_S] == KEY_REPEAT || joystick_down || App->input->controller_1.s_button))
		{
			position.y += speed;
		}

		if ((App->input->GetKey[SDL_SCANCODE_D] == KEY_REPEAT || joystick_right || App->input->controller_1.d_button))
		{
			position.x += speed;
			if (current_animation != &right)
			{
				right.Reset();
				current_animation = &right;
			}

		}

		if ((App->input->GetKey[SDL_SCANCODE_A] == KEY_REPEAT || joystick_left || App->input->controller_1.a_button))
		{
			position.x -= speed;
			if (current_animation != &left)
			{
				left.Reset();
				current_animation = &left;
			}

		}



		if ((App->input->GetKey[SDL_SCANCODE_D] == KEY_IDLE)
			&& (App->input->GetKey[SDL_SCANCODE_A] == KEY_IDLE) && joystick_left == false && joystick_right == false)
		{
			current_animation = &idle;
		}
		if (App->input->GetKey[SDL_SCANCODE_D] == KEY_REPEAT
			&& App->input->GetKey[SDL_SCANCODE_A] == KEY_REPEAT && joystick_left == false && joystick_right == false)
		{
			current_animation = &idle;
		}

		col->rect = { (int)position.x + god_mode * 250, (int)position.y, 19, 28 };

	

	/*else if (SDL_GetTicks() > death_time && death_time != -1 && destroyed == false)
	{

		horse_jump = false;
	}*/


	// Draw everything --------------------------------------
	
	sprintf_s(scores, 8, "%7d", score);
	App->fonts->BlitText(8,8,font_score,scores);

	if(destroyed == false)
		App->render->Blit(graphics, (int)position.x, (int)position.y, &(current_animation->GetCurrentFrame()));

	//Draw HUD(lifes / powerups)---------------------------------

	if (App->input->GetKey[SDL_SCANCODE_G] == KEY_DOWN)
	{
		switch (god_mode)
		{
		case 0:
		{god_mode = 1;
		break;
		}

		case 1:
		{
			god_mode = 0;
			break;
		}
		}
	}


//	death_time = SDL_GetTicks();

	if (lifes != 0)
	{
		App->fade->FadeToBlack((j1Module*)App->scene_space, (j1Module*)App->scene_transit, 0.3f);
		//death = false;
	}
	else
	{
		lifes = 3;
		LOG("OUT OF LIFES");
		App->fade->FadeToBlack((j1Module*)App->scene_space, (j1Module*)App->scene_gameover);
		//death = false;
	}


if (App->input->GetKey[SDL_SCANCODE_N] == KEY_REPEAT&& App->fade->IsFading() == false)
{
	App->fade->FadeToBlack((j1Module*)App->scene_space, (j1Module*)App->scene_gameover);
}
if (App->input->GetKey[SDL_SCANCODE_M] == KEY_REPEAT && destroyed == false && App->fade->IsFading() == false)
{
	lifes -= 1;
	App->particles->AddParticle(App->particles->player_death, position.x, position.y, COLLIDER_NONE);
	destroyed = true;
}

return true;

}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	//death_time = SDL_GetTicks();
	if (c1 == col && c2->type == COLLIDER_ENEMY_SHOT
		 && App->fade->IsFading() == false)
	{
			lifes -= 1;

			if (destroyed == false)
			{
				App->particles->AddParticle(App->particles->player_death, position.x, position.y, COLLIDER_NONE);
				//death_time = -1;
			}

			else death = false;
			destroyed = true;
	}

}

