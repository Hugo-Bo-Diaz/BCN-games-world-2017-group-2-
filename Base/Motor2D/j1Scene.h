#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Physic.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	int counter = 0;
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(const pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void ResetLoad();

private:
	SDL_Texture* img;

	p2List<p2SString*> maps_to_load;
};

#endif // __j1SCENE_H__