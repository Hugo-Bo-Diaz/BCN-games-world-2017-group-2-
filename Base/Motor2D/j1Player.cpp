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
#include "j1Scene.h"

#include<stdio.h>
#include<math.h>

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

	Local_config = config;
	
	return ret;
}
// Load assets
bool j1Player::Start()
{
	bool ret = true;

	// Inicializar lo necesario del jugador, crear los personajes en el mapa

	ret = LoadProperties(Local_config.child("properties"));



	pugi::xml_node node = App->sprites.child("sprites").child("player");
	ret = LoadSprites(node);


	return ret;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");
	/*App->audio->Unload(audio_shot);
	
	App->fonts->UnLoad(font_score);*/
	App->tex->UnLoad(characters[0].graphics);
	App->tex->UnLoad(characters[1].graphics);


	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	characters[0].moving = false;

	characters[1].moving = false;


	
	distance = sqrt(pow(characters[0].real_position.x - characters[1].real_position.x, 2) + pow(characters[0].real_position.y - characters[1].real_position.y, 2));

	if (!characters[0].jumping)
	{ 
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			Jump(character_controll);
			characters[0].jumping = true;
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

/*		
		Animation* test = characters[0].FindAnimByName("idle_happy");
		App->render->Blit(characters[0].graphics, characters[0].real_position.x, characters[0].real_position.y, &test->frames[1]);
*/

		if (distance < minimum_distance)
		{
			if(!time_taken)
			{ 
			time_taker = SDL_GetTicks();
			time_taken = true;
			}

			if ((SDL_GetTicks() - time_taker) > time_to_lower)
			{
				happyness++;

				if (happyness >= maximum_happyness)
				{
					happyness = maximum_happyness;
				}
				time_taken = false;

			}
		}


		if (distance > minimum_distance)
		{
			if (!time_taken)
			{
				time_taker = SDL_GetTicks();
				time_taken = true;
			}

			if ((SDL_GetTicks() - time_taker) > time_to_lower)
			{
				happyness--;
				if (happyness <= 0)
				{
					happyness = 0;
				}
				time_taken = false;

			}
		}

		UpdtHappy();
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

bool j1Player::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;

	pugi::xml_node sheet = sprite_node.child("sheet");

	for (int i = 0; sheet.attribute("source").as_string() != ""; i++) {
		p2SString source = sheet.attribute("source").as_string();
		characters[i].graphics = App->tex->Load(source.GetString());
		pugi::xml_node anima = sheet.child("animation");

		while (anima.attribute("name").as_string() != "") {
			Animation* anim = new Animation;
			
			anim->name.create(anima.attribute("name").as_string());
			anim->loop = anima.attribute("loop").as_bool();
			anim->speed = anima.attribute("speed").as_float();
			
			pugi::xml_node image = anima.child("image");
			for (int j = 0; image.attribute("w").as_int() != 0; j++) {
				anim->frames[j].x = image.attribute("x").as_uint();
				anim->frames[j].y = image.attribute("y").as_uint();
				anim->frames[j].w = image.attribute("w").as_uint();
				anim->frames[j].h = image.attribute("h").as_uint();
				image = image.next_sibling("image");
			}
			characters[i].animations.add(anim);
			anima = anima.next_sibling("animation");
		}
		sheet = sheet.next_sibling("sheet");
		
	}

	return ret;
}

void j1Player::UpdtHappy() {
	// Happyness -> Value     /////    Happiness -> Type of face show (UI)
	if ((happyness * 100) / maximum_happyness >= 66)
		happiness = 0; 
	else if ((happyness * 100) / maximum_happyness >= 33)
		happiness = 1;
	else if ((happyness * 100) / maximum_happyness < 33 && (happyness * 100) / maximum_happyness > 1)
		happiness = 2;
	else {
		App->scene->ResetLoad();
	}

	if (!characters[1].player_anchor)
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

}


bool j1Player::LoadProperties(const pugi::xml_node& property_node) {
	
	bool ret = true;
	// Cargar cosas Happy
	pugi::xml_node char_node = property_node.child("char");

	for (int i = 0; char_node.attribute("name").as_string() != ""; i++) {
		characters[i].real_position.x = char_node.child("position").attribute("x").as_int();//READ HERE FROM XML
		characters[i].real_position.y = char_node.child("position").attribute("y").as_int();//READ HERE FROM XML

		characters[i].render_scale = char_node.child("render_scale").attribute("value").as_float();

		characters[i].player = App->physic->CreateRectangle(characters[i].real_position.x, characters[i].real_position.y, char_node.child("coll_size").attribute("w").as_int(), char_node.child("coll_size").attribute("h").as_int(), b2_dynamicBody, true, PLAYER);

		characters[i].jump_force = char_node.child("jump_force").attribute("value").as_float();
		characters[i].speed = char_node.child("max_speed").attribute("value").as_float(); //cambiar con xml
																										// Cargar aqui los xmls de sprites y de posicion de personajes en el mapa
																										// Propiedades por xml...
		LOG("Loading player");
		if (char_node.child("slide_force").attribute("value").as_bool() == true) {
			characters[i].player_sliding = App->physic->CreateRectangle(1000, 1000, 30, 25, b2_dynamicBody, true, PLAYER);
			characters[i].player_sliding->body->SetActive(false);
		}
		else {
			characters[i].player = App->physic->CreateRectangle(characters[0].real_position.x + 40, characters[0].real_position.y, 30, 50, b2_dynamicBody, true, PLAYER);
			characters[i].player_anchor = App->physic->CreateRectangle(100000, 10000, 30, 50, b2_staticBody, true, GROUND);
			characters[i].player_anchor->body->SetActive(false);
		}
		char_node = char_node.next_sibling("char");
	}
	
	return ret;
}

