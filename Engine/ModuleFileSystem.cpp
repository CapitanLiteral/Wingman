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

	std::string writeLocalPath = SDL_GetBasePath();
	writeLocalPath.append("..");

	if (PHYSFS_setWriteDir(writeLocalPath.c_str()) == 0)
	{
		SDL_Log("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		SDL_Log("Writing directory is %s\n", writeLocalPath.c_str());
		addPath(writeLocalPath.c_str(), getSaveLocalDirectory());
	}

	std::string path;
	path.assign(LOCAL_SAVE_DIRECTORY);
	if (!exist("data"))
		makeDirectory("data");

	if (!exist("root/data/assets"))
		makeDirectory("data/assets");
	if (!exist("root/data/config"))
		makeDirectory("data/config");
	if (!exist("root/data/config/config.json"))
	{
		char* buffer;
		
#ifdef _DEBUG
		load("/root/Debug_Wingman/default_config.json", &buffer);
#else
		load("/root/Release_Wingman/default_config.json", buffer);
#endif
		std::string output(buffer);
		RELEASE_ARRAY(buffer);
		save("data/config/config.json", output.c_str(), output.size());
		
	}
	

	if (!exist("root/data/library"))
		makeDirectory("data/library");

	if (!exist("root/data/assets/scene"))
		makeDirectory("data/assets/scene");
	if (!exist("root/data/assets/fbx"))
		makeDirectory("data/assets/fbx");
	if (!exist("root/data/assets/textures"))
		makeDirectory("data/assets/textures");

	if (!exist("root/data/library/meshes"))
		makeDirectory("data/library/meshes");
	if (!exist("root/data/library/textures"))
		makeDirectory("data/library/textures");
	if (!exist("root/data/library/prefabs"))
		makeDirectory("data/library/prefabs");

	return ret;
}
bool ModuleFileSystem::CleanUp()
{
	SDL_Log("FileSystem: CleanUp.");
	return true;
}
bool ModuleFileSystem::loadConfig()
{
	char* buffer;
	load("root/Debug_Wingman/default_config.json", &buffer);
	std::string input(buffer);
	RELEASE_ARRAY(buffer);
	Json::Reader reader;
	if (!reader.parse(input, directories))
		return false;
	
	directories = directories.get("file_system", 0);
#pragma region Debug
	std::string output;
	Json::StyledWriter writer;
	output = writer.write(directories);
	SDL_Log("Directories:\n%s", output.c_str());
#pragma endregion
	
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
void ModuleFileSystem::Serialize(Json::Value & root)
{
}
void ModuleFileSystem::Deserialize(Json::Value & root)
{
}
const char* ModuleFileSystem::getBasePath()
{
	return PHYSFS_getBaseDir();
}
uint ModuleFileSystem::getFilesOnDir(const char* dir, std::vector<std::string>& files)
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != NULL; ++it)
	{
		files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}
uint ModuleFileSystem::getFilesOnDir(const char* dir, std::vector<std::string>& files, std::string extension)
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != NULL; ++it)
	{
		if (getFileExtension((*it)) == extension)
		{
			files.push_back(*it);
			++ret;
		}		
	}

	PHYSFS_freeList(rc);

	return ret;
}
std::string ModuleFileSystem::getFileExtension(std::string file)
{
	std::string extension;
	for (std::string::reverse_iterator iterator = file.rbegin(); 
			iterator != file.rend(); iterator++)
	{
		if ((*iterator) == '.')
		{
			break;
		}
		extension.push_back((*iterator));
	}
	std::reverse(extension.begin(), extension.end());
	return extension;
}
std::string ModuleFileSystem::removeExtension(const std::string& filename)
{
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

std::string ModuleFileSystem::addExtension(const std::string & filename, const std::string & extension)
{
	std::string output;
	output.append(filename);
	output.append(extension);
	return output;
}
