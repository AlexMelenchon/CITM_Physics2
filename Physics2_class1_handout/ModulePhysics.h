#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#define METERS_TO_PIXELS(a)(a*SCREEN_WIDTH/40)
#define PIXELS_TO_METERS(a)(a*40/SCREEN_WIDTH)



class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:

	bool debug;
	b2World* world = nullptr;

};