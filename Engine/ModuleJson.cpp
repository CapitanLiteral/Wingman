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
	if(App->fs->load("root/data/config/config.json", &buffer))
	{
		std::string string;
		string.assign(buffer);
		std::stringstream stream;
		stream << buffer;

		//SDL_Log("--------------------- CAJUN --------------------- \n%s", stream.str().c_str());
		Json::Value root;
		Json::Reader reader;
		if (reader.parse(stream, root))
		{
			SDL_Log("--------------------- JsonCPP --------------------%s", root.get("Value1", "NoItem").asCString());
			Json::Value beer = root.get("Beers", "NoBeer");
			const Json::Value& beers = root["Beers"];

			for (Json::ValueConstIterator it = beers.begin(); it != beers.end(); it++)
			{
				SDL_Log("--------------------- JsonCPP --------------------%s", (*it).get("Name", "NoItem").asCString());
				const char* buffer;
				buffer = (*it).get("Name", "NoItem").asCString();
				SDL_Log("--------------------- JsonCPP --------------------%s", buffer);
				std::string string;
				string.append((*it).get("Name", "NoItem").asString());
				SDL_Log("--------------------- JsonCPP --------------------%s", string.c_str());
			}
		
		}
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


