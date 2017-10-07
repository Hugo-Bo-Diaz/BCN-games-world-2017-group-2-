#include "p2Defs.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Physic.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1Scene.h"
#include "math.h"
#include "j1Window.h"
#include "j1Player.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

typedef signed int int32;

j1Physics::j1Physics() : j1Module()
{
	world = NULL;
	debug = true;
	name.create("physics");
}

// Destructor
j1Physics::~j1Physics()
{
}

bool j1Physics::Awake(const pugi::xml_node& config)
{
	bool ret = true;

	world = new b2World({config.child("gravity").attribute("x").as_float(),config.child("gravity").attribute("y").as_float()});
	// TODO 3: You need to make ModulePhysics class a contact listener

	world->SetContactListener(App->physic);

	// big static circle as "ground" in the middle of the screen
	int x = 0;
	int y = 600;
	int diameter = config.child("test_platform").attribute("h").as_int();


	//CreateCircle(x, y, diameter * 0.5f, b2_staticBody);

	CreateRectangle(x, y, config.child("test_platform").attribute("w").as_int(), config.child("test_platform").attribute("h").as_int(), b2_staticBody);
	CreateRectangle(x + 1000, y, config.child("test_platform").attribute("w").as_int(), config.child("test_platform").attribute("h").as_int(), b2_staticBody);
	CreateRectangle(x + 2000, y, config.child("test_platform").attribute("w").as_int(), config.child("test_platform").attribute("h").as_int(), b2_staticBody);
	CreateRectangle(640,2804, 80, 80, b2_dynamicBody, true, GROUND);
	return ret;
}
bool j1Physics::Start()
{
	LOG("Creating Physics 2D environment");

	return true;
}

// 
bool j1Physics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	// TODO: HomeWork
	/*
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
	}
	*/

	return true;
}

PhysBody* j1Physics::CreateCircle(int x, int y, int radius,b2BodyType body_type )
{
	b2BodyDef body;
	body.type = body_type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;


	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	PhysBody* pbody = new PhysBody();

	
	pbody->body = b;
	pbody->width = pbody->height = radius;
	pbody->module_player = App->player;
    b->SetUserData(pbody);
	return pbody;
}

PhysBody* j1Physics::CreateRectangle(int x, int y, int width, int height, b2BodyType body_type, bool give_collision, Phys_type col_type)
{
	b2BodyDef body;
	body.type = body_type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0;
	if (col_type == PLAYER)
	{
		fixture.filter.groupIndex = -1;

	}
	if (col_type == GROUND)
	{
		fixture.filter.groupIndex = 1;
	}

	b->CreateFixture(&fixture);

	PhysBody* pbody;
	//delete pbody;
	pbody = new PhysBody();
	
	pbody->body = b;
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;
	pbody->type = col_type;
	if(give_collision)
	pbody->module_player = App->player;
	
	b->SetUserData(pbody);

	return pbody;
}

PhysBody* j1Physics::CreateChain(int x, int y, int* points, int size, b2BodyType body_type)
{
	b2BodyDef body;
	body.type = body_type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;
	b->SetUserData(pbody);

	return pbody;
}

// 
bool j1Physics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return true;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return true;
}


// Called before quitting
bool j1Physics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	// TODO 1: Write the code to return true in case the point
	// is inside ANY of the shapes contained by this body

	b2Fixture* iterator;
	b2Vec2 position(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	bool ret = false;
	iterator = body->GetFixtureList();

	while (iterator != nullptr && !ret)
	{
		ret = iterator->TestPoint(position);
		iterator = iterator->GetNext();
	}

	
	return ret;
}

void PhysBody::SetVelocity(b2Vec2 velocity_) 
{
	b2Vec2 velocity;
	velocity.x = PIXEL_TO_METERS(velocity_.x);
	velocity.y = body->GetLinearVelocity().y;
	body->SetLinearVelocity(velocity);
	
}
void PhysBody::SetActive(bool active)
{
	body->SetActive(active);

}
void PhysBody::SetPosition(int x, int y)
{
	b2Vec2 position;
	position.x = PIXEL_TO_METERS(x);
	position.y = PIXEL_TO_METERS(y);
	body->SetTransform(position, 0);
}
void PhysBody::ApplyForce(b2Vec2 force)
{
	body->ApplyForceToCenter(force, true);
}
void PhysBody::TateQuieto()
{
	b2Vec2 quietoparau;
	quietoparau.x = body->GetPosition().x;
	quietoparau.y = body->GetPosition().y;
	body->SetTransform(quietoparau, 0);
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	// TODO 2: Write code to test a ray cast between both points provided. If not hit return -1
	// if hit, fill normal_x and normal_y and return the distance between x1,y1 and it's colliding point


	b2Fixture* iterator;
	iterator = body->GetFixtureList();
	b2RayCastInput raycasti;
	b2RayCastOutput raycasto;
	raycasti.p1 = b2Vec2(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	raycasti.p2 = b2Vec2(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	raycasti.maxFraction = 3;
	bool spoted = false;

	while (iterator != nullptr && !spoted)
	{
		spoted = iterator->RayCast(&raycasto, raycasti,0);
		iterator = iterator->GetNext();
	}

	if (!spoted)
		return -1;
	else
	{ 

	normal_x = raycasto.normal.x;
	normal_y = raycasto.normal.y;

	b2Vec2 distance_p1_cp((raycasti.p2.x - raycasti.p1.x)*raycasto.fraction, (raycasti.p2.y - raycasti.p1.y)*raycasto.fraction);

	

	return distance_p1_cp.Length();
   }
	
}

void j1Physics::BeginContact(b2Contact* contact)
{
	PhysBody* PhysBodyA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();

	PhysBody* PhysBodyB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

   if (PhysBodyB->module_player != nullptr)
	{
		PhysBodyB->module_player->OnCollision(PhysBodyB, PhysBodyA);
	}
	if (PhysBodyA->module_player != nullptr)
	{
		PhysBodyA->module_player->OnCollision(PhysBodyA, PhysBodyB);
	}
	//LLamar al "OnCollision" de los módulos que contengan las diferentes strucs
}
void j1Physics::EndContact(b2Contact* contact)
{
	/*PhysBody* PhysBodyA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();

	PhysBody* PhysBodyB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (PhysBodyB->module_player != nullptr)
	{
		if (PhysBodyA->type == GROUND)
			PhysBodyB->module_player->characters[0].jumping = true;
	}

	if (PhysBodyA->module_player != nullptr)
	{
		if (PhysBodyB->type == GROUND)
			PhysBodyA->module_player->characters[0].jumping = true;
	}*/
	//LLamar al "OnCollision" de los módulos que contengan las diferentes strucs
}


// TODO 3

// TODO 7: Call the listeners that are not NULL