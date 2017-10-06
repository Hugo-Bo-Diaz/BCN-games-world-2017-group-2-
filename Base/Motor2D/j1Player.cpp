#include "p2defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1Physic.h"

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

	graphics = App->tex->Load("textures/test.png");

	destroyed = false;
	position.x = 30;//READ HERE FROM XML
	position.y = 0;//READ HERE FROM XML
	
	audio_shot = App->audio->LoadFx("gunsmoke/shotfx.wav");

	//col = App->collision->AddCollider({(int)position.x, (int)position.y, 19, 28}, COLLIDER_PLAYER, this);

	player = App->physic->CreateRectangle(position.x, position.y, 30, 50, b2_dynamicBody);
	
	jump_force = -250;
	speed = 250; //cambiar con xml

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	/*App->audio->Unload(audio_shot);
	App->tex->Unload(graphics);
	App->fonts->UnLoad(font_score);*/

	if (col != nullptr)
		//col->to_delete = true;

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	moving = false;

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

		if (App->input->controller_1.right_joystick.x != 0 && App->input->controller_1.right_joystick.y != 0)
		{
			//calculate angle
			joystick_angle = atan2(App->input->controller_1.right_joystick.y, App->input->controller_1.right_joystick.x);
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN|| joystick_up || App->input->controller_1.jump)
		{
			b2Vec2 jump_force_vector;
			jump_force_vector.x = 0;
			jump_force_vector.y = jump_force;

			player->ApplyForce(jump_force_vector);
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || joystick_down)
		{
			
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || joystick_right)
		{
			b2Vec2 speedo;
			speedo.x = speed;
			speedo.y = 0;
			player->SetVelocity(speedo);
			moving = true;
			if (current_animation != &right)
			{
				right.Reset();
				current_animation = &right;
			}

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || joystick_left)
		{
			b2Vec2 speedo;
			speedo.x = -speed;
			speedo.y = 0;
			player->SetVelocity(speedo);
			moving = true;
			if (current_animation != &left)
			{
				left.Reset();
				current_animation = &left;
			}

		}



		if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			&& (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) && joystick_left == false && joystick_right == false)
		{
			current_animation = &idle;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			&& App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && joystick_left == false && joystick_right == false)
		{
			current_animation = &idle;
		}

	

	//Move player
		if (!moving)
		{
			b2Vec2 speedo;
			speedo.x = 0;
			speedo.y = 0;
			player->SetVelocity(speedo);
	    }

		player->TateQuieto();
	// Draw everything --------------------------------------
	
		App->render->Blit(sprites, position.x, position.y);
	//Draw HUD(lifes / powerups)---------------------------------

	

	return true;

	}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{

}

