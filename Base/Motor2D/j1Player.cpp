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
	name.create("player");
	
}

j1Player::~j1Player()
{}

// Awake, actually load things?
bool j1Player::Awake(const pugi::xml_node& config)
{
	bool ret = true;
	characters[0].real_position.x = 30;//READ HERE FROM XML
	characters[0].real_position.y = 0;//READ HERE FROM XML

	characters[0].player = App->physic->CreateRectangle(characters[0].real_position.x, characters[0].real_position.y, 30, 50, b2_dynamicBody, true, PLAYER);

	characters[0].jump_force = config.child("physics").child("jump_force").attribute("value").as_float();
	characters[0].speed = config.child("physics").child("max_speed").attribute("value").as_float(); //cambiar con xml
	// Cargar aqui los xmls de sprites y de posicion de personajes en el mapa
	// Propiedades por xml...
	LOG("Loading player");
	characters[0].player_sliding = App->physic->CreateRectangle(1000, 1000, 30, 25, b2_dynamicBody, true, PLAYER);
	characters[0].player_sliding->body->SetActive(false);

	characters[1].player = App->physic->CreateRectangle(characters[0].real_position.x + 40, characters[0].real_position.y, 30, 50, b2_dynamicBody, true, PLAYER);
	characters[1].player_anchor = App->physic->CreateRectangle(100000, 10000, 30, 50, b2_staticBody, true, GROUND);
	characters[1].player_anchor->body->SetActive(false);



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
	App->tex->UnLoad(characters[0].graphics);


	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	characters[0].moving = false;

	characters[1].moving = false;


	if (!characters[0].jumping)
	{ 
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			Jump(character_controll);
		}
	}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			if (!character_controll)
			SlideStart();
			else
			{
				if(!characters[0].jumping)
				{ 
				if(!characters[1].anchored)
				AnchorStart();
				else
				AnchorEnd();
				}
			}
			

			//Gordete anchor
		}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_UP)
		{
			if (!character_controll)
				SlideEnd();
			else
			{
				
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			GoRight(character_controll);
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			GoLeft(character_controll);
		}
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			character_controll = !character_controll;
		}

	

	//Move player
		if (!characters[0].moving)
		{
			StopMoving(false);
	    }

		if (!characters[1].moving)
		{
			StopMoving(true);
		}

		characters[0].player->TateQuieto();

		characters[1].player->TateQuieto();
	// Draw everything --------------------------------------


		if (characters[0].sliding)
		{
			int x, y;
			characters[0].player_sliding->GetPosition(x, y);
			characters[0].real_position.x = x;
			characters[0].real_position.y = y;
		}
		else
		{
			int x, y;
			characters[0].player->GetPosition(x, y);
			characters[0].real_position.x = x;
			characters[0].real_position.y = y;
		}


		if (characters[1].player_anchor)
		{
			int x, y;
			characters[1].player_anchor->GetPosition(x, y);
			characters[1].real_position.x = x;
			characters[1].real_position.y = y;
		}
		else
		{
			int x, y;
			characters[1].player->GetPosition(x, y);
			characters[1].real_position.x = x;
			characters[1].real_position.y = y;
		}
	
	//App->render->Blit(sprites, position.x, position.y);
//Draw HUD(lifes / powerups)---------------------------------



return true;

}

void j1Player::OnCollision(PhysBody* player, PhysBody* other)
{
	if (other->type == GROUND)
	{
		characters[0].jumping = false;
    }
}

bool j1Player::Load(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}

bool j1Player::Save(const pugi::xml_node& savegame) {
	bool ret = true;

	return ret;
}
void j1Player::Jump(bool character)
{
	b2Vec2 jump_force_vector;
	
	jump_force_vector.x = 0;
	jump_force_vector.y = characters[0].jump_force;
	if(character)
	{ 
		characters[1].player->ApplyForce(jump_force_vector);
		characters[1].jumping = true;
	}
	else
	{
		characters[0].player->ApplyForce(jump_force_vector);
		characters[0].jumping = true;
	}
}

void j1Player::SlideStart()
{
		characters[0].sliding = true;
		b2Vec2 position_;
		position_ = characters[0].player->body->GetPosition();
		characters[0].player->SetActive(false);
		characters[0].player->SetPosition(10000, 10000);
		characters[0].player_sliding->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y) + characters[0].player->height / 4 + 5);
		characters[0].player_sliding->SetActive(true);
		b2Vec2 velocity;
		if (characters[0].face_right)
		velocity.x = characters[0].speed + 100;
		else
		velocity.x = -(characters[0].speed + 100);

		velocity.y = 0;
		characters[0].player_sliding->SetVelocity(velocity);
}
void j1Player::SlideEnd()
{
	characters[0].sliding = false;

	b2Vec2 position_;
	position_ = characters[0].player_sliding->body->GetPosition();
	characters[0].player_sliding->SetActive(false);
	characters[0].player_sliding->SetPosition(10000, 10000);
	characters[0].player->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y) - characters[0].player_sliding->height);
	characters[0].player->SetActive(true);
	b2Vec2 velocity;
	velocity.x = 0;
	velocity.y = 0;
	characters[0].player->SetVelocity(velocity);
}

void j1Player::GoLeft(bool character)
{
	b2Vec2 speedo;
	speedo.x = -characters[0].speed;
	speedo.y = 0;

	if (character)
	{
		characters[1].player->SetVelocity(speedo);
		characters[1].moving = true;
		characters[1].face_right = false;
	}
	else
	{
		characters[0].player->SetVelocity(speedo);
		characters[0].moving = true;
		characters[0].face_right = false;
	}
}

void j1Player::GoRight(bool character)
{
	b2Vec2 speedo;
	speedo.x = characters[0].speed;
	speedo.y = 0;

	if(character)
	{ 
		characters[1].player->SetVelocity(speedo);
		characters[1].moving = true;
		characters[1].face_right = true;
	}
	else
	{
		characters[0].player->SetVelocity(speedo);
		characters[0].moving = true;
		characters[0].face_right = true;
	}
}

void j1Player::StopMoving(bool character)
{
	b2Vec2 speedo;
	speedo.x = 0;
	speedo.y = 0;
	if(character)
	characters[1].player->SetVelocity(speedo);
	else
	characters[0].player->SetVelocity(speedo);

}

void j1Player::AnchorStart()
{
	characters[1].anchored = true;
	b2Vec2 position_;
	position_ = characters[1].player->body->GetPosition();
	characters[1].player->SetActive(false);
	characters[1].player->SetPosition(10000, 10000);
	characters[1].player_anchor->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y));
	characters[1].player_anchor->SetActive(true);
	b2Vec2 velocity;
		
	velocity.x = 0;
	velocity.y = 0;
	characters[1].player_anchor->SetVelocity(velocity);
	characters[0].jumping = false;
}

void j1Player::AnchorEnd()
{
	characters[1].anchored = false;
	b2Vec2 position_;
	position_ = characters[1].player_anchor->body->GetPosition();
	characters[1].player_anchor->SetActive(false);
	characters[1].player_anchor->SetPosition(10000, 10000);
	characters[1].player->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y));
	characters[1].player->SetActive(true);
	b2Vec2 velocity;
	velocity.x = 0;
	velocity.y = 0;
	characters[1].player->SetVelocity(velocity);
}

