#include "p2defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
//#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"

#include<stdio.h>

j1Player::j1Player()
{
	name.create("player");
	
}

j1Player::~j1Player()
{}

// Awake, actually load things?
bool j1Player::Awake(const pugi::xml_node& config)
{
	bool ret = true;

	// Cargar aqui los xmls de sprites y de posicion de personajes en el mapa
	// Propiedades por xml...
	LOG("Loading player");

	graphics = App->tex->Load("textures/test.png");

	destroyed = false;
	position.x = 0;//READ HERE FROM XML
	position.y = 0;//READ HERE FROM XML

	audio_shot = App->audio->LoadFx("gunsmoke/shotfx.wav");

	//col = App->collision->AddCollider({(int)position.x, (int)position.y, 19, 28}, COLLIDER_PLAYER, this);
	// idle animation
	
	idle.PushBack({ 4, 8, 19, 29 });
	idle.PushBack({ 25, 8, 20, 29 });
	idle.PushBack({ 48, 8, 19, 29 });
	idle.PushBack({ 70, 8, 20, 29 });
	idle.PushBack({ 91, 8, 21, 29 });
	idle.speed = 0.2f;

	return ret;
}
// Load assets
bool j1Player::Start()
{
	bool ret = true;

	// Inicializar lo necesario del jugador, crear los personajes en el mapa

	return ret;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	/*App->audio->Unload(audio_shot);
	
	App->fonts->UnLoad(font_score);*/
	App->tex->UnLoad(graphics);

	if (col != nullptr)
		//col->to_delete = true;

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
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

	if (App->input->controller_1.right_joystick.x != 0 && App->input->controller_1.right_joystick.y != 0)
	{
		//calculate angle
		joystick_angle = atan2(App->input->controller_1.right_joystick.y, App->input->controller_1.right_joystick.x);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || joystick_up || App->input->controller_1.jump)
	{
		position.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || joystick_down)
	{
		position.y += speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || joystick_right)
	{
		position.x += speed;
		if (current_animation != &right)
		{
			right.Reset();
			current_animation = &right;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || joystick_left)
	{
		position.x -= speed;
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



// Draw everything --------------------------------------
	
	App->render->Blit(sprites, position.x, position.y);
//Draw HUD(lifes / powerups)---------------------------------

	

return true;

}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{

}

bool j1Player::Load(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}

bool j1Player::Save(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}

