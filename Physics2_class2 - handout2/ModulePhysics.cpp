#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circlelist.add(CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 50));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		CreateVox(App->input->GetMouseX(), App->input->GetMouseY());
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		CreateRick(App->input->GetMouseX(), App->input->GetMouseY());
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

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
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
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
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
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
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}


BodyPointer ModulePhysics::CreateCircle(int mouseX, int mouseY, int diameter){
	b2BodyDef body;
	body.type = b2_dynamicBody;
	float radius = PIXEL_TO_METERS(diameter/2);
	body.position.Set(PIXEL_TO_METERS(mouseX), PIXEL_TO_METERS(mouseY));

	b2Body* b = world->CreateBody(&body);

	b->GetPosition();
	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&shape, 1.0f);

	BodyPointer toReturn;
	toReturn.pointer = b;

	return toReturn;
}

void ModulePhysics::CreateVox(int mouseX, int mouseY) {

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(mouseX), PIXEL_TO_METERS(mouseY));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape polygon;
	polygon.SetAsBox(PIXEL_TO_METERS(20), PIXEL_TO_METERS(20));

	b2FixtureDef fixture;
	fixture.shape = &polygon;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);
}

void ModulePhysics::CreateRick(int mouseX, int mouseY) {


	// Pivot 0, 0
	int rick_head[50] = {
		13, 36,
		42, 38,
		40, 1,
		74, 31,
		87, 5,
		95, 38,
		111, 34,
		104, 58,
		116, 66,
		109, 75,
		106, 94,
		110, 100,
		103, 106,
		106, 122,
		78, 148,
		51, 137,
		46, 132,
		35, 136,
		39, 126,
		23, 123,
		31, 116,
		10, 103,
		29, 90,
		1, 75,
		30, 63
	};



	b2Vec2 vs[25];

	uint y = 0;
	for (uint i = 0; i <= 49; i = i + 2)
	{
		vs[y].Set(PIXEL_TO_METERS(rick_head[i]), PIXEL_TO_METERS(rick_head[i + 1]));
		y++;

	}
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(mouseX), PIXEL_TO_METERS(mouseY));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape chain;
	chain.CreateLoop(vs, 25);

	b2FixtureDef fixture;
	fixture.shape = &chain;

	b->CreateFixture(&fixture);


}


b2Vec2 BodyPointer::GetBodyPosition()
{
	b2Vec2 aux = pointer->GetPosition();
	(aux.x);
	(aux.y);

	return aux;

}

float BodyPointer::GetRotation()  {
	return RADTODEG * pointer->GetAngle();
}