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

void ModuleJson::read(module moduleType)
{
	char* buffer;
	if(App->fs->load("root/data/config/config.json", &buffer))
	{
		std::string string;
		string.assign(buffer);
		std::stringstream stream;
		stream << buffer;

		Json::Value root;
		Json::Reader reader;

		if (moduleType == BEER_TEST)
		{
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
		else if (moduleType == WINDOW)
		{
			if (reader.parse(stream, root))
			{
				Json::Value windowConfig = root.get("window", "ERR_NO_WINDOW_CONFIG");
				const Json::Value& windowConfigParams = root["window"];

				//for (Json::ValueConstIterator it = windowConfigParams.begin(); it != windowConfigParams.end(); it++)
				//{
				//	SDL_Log("--------------------- JsonCPP --------------------%s", (*it).asCString());
				//}
			}
		}
		
		
	}
	else
	{
		SDL_Log("Error loading config.");
	}
	
	//root.get("Delicious Beers", "NoItem");
}

bool ModuleJson::Init()
{
	read(WINDOW);
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


