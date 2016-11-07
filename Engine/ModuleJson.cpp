#include "Globals.h"
#include "Application.h"

#include "ModuleJson.h"
#include "ModuleFileSystem.h"

#include <json\json.h>

#include <sstream>
#include <vector>

ModuleJson::ModuleJson(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}


ModuleJson::~ModuleJson()
{
}

void ModuleJson::read()
{
	char* buffer;
	App->fs->load("saveLocal/data/config/config.json", &buffer);
	std::string string;
	string.assign(buffer);
	std::stringstream stream;
	stream << buffer;

	//SDL_Log("--------------------- CAJUN --------------------- \n%s", stream.str().c_str());
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(stream, root))
	{
		SDL_Log("--------------------- JsonCPP --------------------\n%s", root.get("Value1", "NoItem").asCString());

		
	}

	
	//root.get("Delicious Beers", "NoItem");
}

bool ModuleJson::Init()
{
	read();
	return true;
}

bool ModuleJson::Start()
{
	return true;
}

update_status ModuleJson::Update()
{
	return UPDATE_CONTINUE;
}


