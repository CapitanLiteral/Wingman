#include "Application.h"
#include "Globals.h"
#include "ModuleFileSystem.h"

#include "PhysFS\include\physfs.h"
#include "SDL\include\SDL.h"

#include <string>

#pragma comment( lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(App, start_enabled)
{
	SDL_Log("FileSystem: Creation.");
	//name.assign("module_filesystem");

	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	addPath(".");
}


ModuleFileSystem::~ModuleFileSystem()
{
	SDL_Log("FileSystem: Destroying.");
	PHYSFS_deinit();
}


bool ModuleFileSystem::Init()
{
	SDL_Log("FileSystem: init.");
	bool ret = true;

	/*for (pugi::xml_node path = config.child("path"); path; path = path.next_sibling("path"))
	addPath(path.child_value());*/

#pragma region OldWriteDirectory
	//std::string writeAppDataPath = SDL_GetPrefPath(App->getOrganization(), App->getTitle());

	//if (PHYSFS_setWriteDir(writeAppDataPath.c_str()) == 0)
	//{
	//	SDL_Log("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	//}
	//else
	//{
	//	SDL_Log("Writing directory is %s\n", writeAppDataPath);
	//	addPath(writeAppDataPath.c_str(), getSaveAppDataDirectory());
	//}
#pragma endregion


	std::string writeLocalPath = SDL_GetBasePath();
	writeLocalPath.append("..");

	if (PHYSFS_setWriteDir(writeLocalPath.c_str()) == 0)
	{
		SDL_Log("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		SDL_Log("Writing directory is %s\n", writeLocalPath);
		addPath(writeLocalPath.c_str(), getSaveLocalDirectory());
	}
	std::string path;
	path.assign(LOCAL_SAVE_DIRECTORY);
	if (!exist(path.c_str()))
		makeDirectory(LOCAL_SAVE_DIRECTORY);

	if (!exist(ASSETS_DIRECTORY))
		makeDirectory(ASSETS_DIRECTORY, path.c_str());
	if (!exist(CONFIG_DIRECTORY))
		makeDirectory(CONFIG_DIRECTORY, path.c_str());

	path.append("/");
	path.append(ASSETS_DIRECTORY);

	if (!exist(MESHES_DIRECTORY))
		makeDirectory(MESHES_DIRECTORY, path.c_str());

	if (!exist(MATERIAL_DIRECTORY))
		makeDirectory(MATERIAL_DIRECTORY, path.c_str());

	if (!exist(ANIMATION_DIRECTORY))
		makeDirectory(ANIMATION_DIRECTORY, path.c_str());



	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	SDL_Log("FileSystem: CleanUp.");
	return true;
}

bool ModuleFileSystem::addPath(const char* pathOrZip, const char* mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
	{
		SDL_Log("File System error while adding a path or zip(%s): %s.", pathOrZip, PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::exist(const char* file)
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::isDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::makeDirectory(const char* dir, const char* mount)
{
	bool ret = false;

	if (!dir)
		return ret;

	char newDir[128];

	if (mount)
		sprintf_s(newDir, 128, "%s/%s", mount, dir);
	else
		sprintf_s(newDir, 128, dir);

	if (PHYSFS_mkdir(newDir) == 0)
	{
		SDL_Log("Could not make dir: %s. PhsyFs error: %s", newDir, PHYSFS_getLastError());
	}
	else
	{
		SDL_Log("Just created a dir: %s.", newDir);
		ret = true;
	}

	return ret;
}

unsigned int ModuleFileSystem::load(const char* file, char** buffer)const
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);
			{
				if (readed != size)
				{
					SDL_Log("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
					RELEASE(buffer);
				}
				else
				{
					ret = (uint)readed;
				}
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			SDL_Log("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		SDL_Log("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

int closeSdlRwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

SDL_RWops* ModuleFileSystem::load(const char* file)const
{
	char* buffer;
	int size = load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = closeSdlRwops;

		return r;
	}
	else
		return NULL;
}

unsigned int ModuleFileSystem::save(const char* file, const char* buffer, unsigned int size)const
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			SDL_Log("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fsFile) == 0)
			SDL_Log("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		SDL_Log("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

const char* ModuleFileSystem::getBasePath()
{
	return PHYSFS_getBaseDir();
}