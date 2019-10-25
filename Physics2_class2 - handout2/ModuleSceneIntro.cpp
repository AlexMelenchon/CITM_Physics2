#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

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
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	
	// TODO 7: Draw all the circles using "circle" texture

	for (p2List_item<BodyPointer>* ballIterator = App->physics->circlelist.getFirst(); ballIterator != NULL; ballIterator = ballIterator->next)
	{
		b2Vec2 vec = ballIterator->data.GetBodyPosition();
		SDL_Rect aux;
		aux.x = METERS_TO_PIXELS(vec.x);
		aux.y = METERS_TO_PIXELS(vec.y);

		float32 angle = ballIterator->data.GetRotation() ;

		App->renderer->Blit(circle, aux.x-25, aux.y-25,NULL,1.0f, angle);
	}

	return UPDATE_CONTINUE;
}
