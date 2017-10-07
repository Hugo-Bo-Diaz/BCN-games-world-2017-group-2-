#pragma once
#include "J1Module.h"

#include "p2Defs.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class j1Scene;
class j1Player;

enum Phys_type
{
	PLAYER,
	GROUND,
};
// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void SetVelocity(b2Vec2);
	void ApplyForce(b2Vec2);
	void TateQuieto();
	void SetActive(bool);
	void SetPosition(int x, int y);
	

public:
	int width, height;

	int x, y;

	Phys_type type;
	b2Body* body;
	j1Player* module_player = nullptr;
	// TODO 6: Add a pointer to a module that might want to listen to a collision from this body
};

// Module --------------------------------------
// TODO 3: Make module physics inherit from b2ContactListener
// then override void BeginContact(b2Contact* contact)
class j1Physics : public j1Module, public b2ContactListener
{
public:
	j1Physics();
	~j1Physics();

	bool Awake(const pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType body_type);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType body_type, bool give_collision = false, Phys_type col_type = GROUND);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType body_type);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType body_type);

	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);

private:

	bool debug;
	b2World* world;
};