#pragma once
#include "Module.h"


class ModuleJson : public Module
{
public:
	ModuleJson(Application* app, bool start_enabled = true);
	~ModuleJson();

	void print_commits_info(const char *username, const char *repo);
};

