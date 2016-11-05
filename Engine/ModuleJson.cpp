#include "Globals.h"
#include "Application.h"

#include "ModuleJson.h"

#include "Parson\parson.h"
#include "Parson\parson.c"


ModuleJson::ModuleJson(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	print_commits_info("CapitanLiteral", "Wingman");
}


ModuleJson::~ModuleJson()
{
}

void ModuleJson::print_commits_info(const char *username, const char *repo)
{
	
}
