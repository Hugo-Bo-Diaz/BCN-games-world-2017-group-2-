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

	ret = LoadProperties(config.child("properties"));

	pugi::xml_node node = App->sprites.child("player");
	ret = LoadSprites(node);

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


	if (!characters[0].jumping)
	{ 
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			b2Vec2 jump_force_vector;
			jump_force_vector.x = 0;
			jump_force_vector.y = characters[0].jump_force;
			characters[0].player->ApplyForce(jump_force_vector);
			characters[0].jumping = true;
		}
	}


		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
		
			characters[0].sliding = true;
			b2Vec2 position_;
			position_ = characters[0].player->body->GetPosition();
			characters[0].player->SetActive(false);
			characters[0].player->SetPosition(10000, 10000);
			characters[0].player_sliding->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y)+ characters[0].player->height/4+5);
			characters[0].player_sliding->SetActive(true);
			b2Vec2 velocity;
			if (characters[0].face_right)
			velocity.x = characters[0].speed + 100;
			else
			velocity.x = -(characters[0].speed + 100);

			velocity.y = 0;
			characters[0].player_sliding->SetVelocity(velocity);
		}

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_UP)
		{
			characters[0].sliding = false;
		
			b2Vec2 position_;
			position_ = characters[0].player_sliding->body->GetPosition();
			characters[0].player_sliding->SetActive(false);
			characters[0].player_sliding->SetPosition(10000, 10000);
			characters[0].player->SetPosition(METERS_TO_PIXELS(position_.x), METERS_TO_PIXELS(position_.y)- characters[0].player_sliding->height);
			characters[0].player->SetActive(true);
			b2Vec2 velocity;
			velocity.x = 0;
			velocity.y = 0;
			characters[0].player->SetVelocity(velocity);
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			b2Vec2 speedo;
			speedo.x = characters[0].speed;
			speedo.y = 0;
			characters[0].player->SetVelocity(speedo);
			characters[0].moving = true;
			characters[0].face_right = true;

		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			b2Vec2 speedo;
			speedo.x = -characters[0].speed;
			speedo.y = 0;
			characters[0].player->SetVelocity(speedo);
			characters[0].moving = true;
			characters[0].face_right = false;

		}


	

	//Move player
		if (!characters[0].moving)
		{
			b2Vec2 speedo;
			speedo.x = 0;
			speedo.y = 0;
			characters[0].player->SetVelocity(speedo);
	    }

		characters[0].player->TateQuieto();
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

bool j1Player::LoadSprites(const pugi::xml_node& sprite_node) {
	bool ret = true;

	pugi::xml_node anim = sprite_node.child("animation");

	for (int i = 0; sprite_node.attribute("source").as_string() != ""; i++) {
		characters[i].graphics = App->tex->Load(sprite_node.attribute("source").as_string());

		while (anim.attribute("name").as_string() != "") {
			Animation* anima = new Animation;
			
			anima->name.create(anim.attribute("name").as_string());
			anima->loop = anim.attribute("loop").as_bool();
			anima->speed = anim.attribute("speed").as_float();
			
			pugi::xml_node image = anim.child("image");
			for (int j = 0; image.attribute("w").as_int() != 0; j++) {
				anima->frames[j].x = image.attribute("x").as_uint();
				anima->frames[j].y = image.attribute("y").as_uint();
				anima->frames[j].w = image.attribute("w").as_uint();
				anima->frames[j].h = image.attribute("h").as_uint();
				image = image.next_sibling("image");
			}
			characters[i].animations.add(anima);
			anim = anim.next_sibling("animation");
		}

		
	}

	return ret;
}

bool j1Player::LoadProperties(const pugi::xml_node& property_node) {
	
	bool ret = true;
	pugi::xml_node char_node = property_node.child("char");

	for (int i = 0; char_node.attribute("name").as_string() != ""; i++) {
		characters[i].real_position.x = char_node.child("position").attribute("x").as_int();//READ HERE FROM XML
		characters[i].real_position.y = char_node.child("position").attribute("y").as_int();//READ HERE FROM XML

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
		char_node = char_node.next_sibling("char");
	}
	
	return ret;
}
