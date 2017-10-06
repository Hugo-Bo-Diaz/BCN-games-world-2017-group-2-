#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(const pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node node = config.child("map");
	while (node.attribute("source").as_string() != "") {
		p2SString* new_map = new p2SString;
		new_map->create(config.child("map").attribute("source").as_string());
		maps_to_load.add(new_map);
		node = node.next_sibling("map");
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	//img = App->tex->Load("textures/test.png");
	// LOAD MAPS AND MUSIC HERE

	//if (ret == true) ret = App->map->Load("TMX tests/Trial.tmx");
	p2List_item<p2SString*>* item = maps_to_load.start;
	while (ret == true && item != nullptr) {
		App->map->Load(item->data->GetString());
		item = item->next;
	}
	//if (ret == true) ret = App->map->Load("Poner Direccion de Mapa aquí");
	
	if (ret == true) ret = App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	// TODO 2.5: Call load / save methods when pressing l/s

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->Trigger_Load();

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		App->Trigger_Save();

	// TODO 2.Homework Allow for change in volume
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT) {
		App->audio->Increase_Master();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) {
		App->audio->Decrease_Master();
	}

	// Camera movement Inputs

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 10;


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		++counter;
		App->physic->CreateCircle(0+counter, 0, 25, b2_dynamicBody);
		// TODO 8: Make sure to add yourself as collision callback to the circle you creates
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT) {
		App->win->Res.y += (10 * (App->win->Res.y / App->win->Res.x));
		App->win->Res.x += 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		App->win->Res.y -= (10 * (App->win->Res.y / App->win->Res.x));
		App->win->Res.x -= 10;
	}
	
	/*// Change Camera 
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		App->win->UpScale();

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->win->DownScale();*/


	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 3.7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2List_item<Map_info*>* tmp_map = App->map->Maps.start;
	/*p2SString title("Maps:%d MapSize:%dx%d TileSize:%dx%d Tilesets:%d Layers:%d Tiles:%d",
		App->map->Maps.count(),
		tmp_map->data->width, tmp_map->data->height,
		tmp_map->data->tilewidth, tmp_map->data->tileheight,
		tmp_map->data->tilesets.count(),
		tmp_map->data->layers.count(),
		tmp_map->data->layers.start->data->size);
	*/
	//App->win->SetTitle(title.GetString());
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
//
void j1Scene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	LOG("COLLISION!");
}
