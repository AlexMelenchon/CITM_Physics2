#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;



	// TODO 2: Place the camera one unit up in Y and one unit to the right
	// experiment with different camera placements, then use LookAt()
	// to make it look at the center
	App->camera->Move({ 0.0f, 1.0f, 0.0f });


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 1: Create a Plane primitive. This uses the plane formula
	// so you have to express the normal of the plane to create 
	// a plane centered around 0,0. Make that it draw the axis for reference
	Plane plane1(0, 0, 0, 1);

	plane1.Render();

	Cylinder cyl1(10.0f, 10.0f);
	cyl1.wire = true;

	cyl1.Render();

	// TODO 6: Draw a sphere of 0.5f radius around the center
	// Draw somewhere else a cube and a cylinder in wireframe
	Sphere sph2(0.5f);
	sph2.wire = false;

	sph2.Render();


	Sphere ref(0.01f);
	ref.wire = true;
	ref.SetPos (App->camera->Reference.x, App->camera->Reference.y, App->camera->Reference.z);

	ref.Render();

	return UPDATE_CONTINUE;
}