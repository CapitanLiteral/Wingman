#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include "IJsonSerializable.h"
#include "jsoncpp\json\json.h"
#include <map>
#include <string>

struct SDL_RWops;

class ModuleFileSystem : public Module, IJsonSerializable
{
public:
	Json::Value directories;

	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init();
	bool CleanUp();

	bool loadConfig();

	bool addPath(const char* pathOrZip, const char* mountPoint = NULL);
	bool exist(const char* file);
	bool isDirectory(const char* file);
	bool makeDirectory(const char* dir, const char* mount = NULL);
	const char* getSaveAppDataDirectory()const { return "saveAppData/"; }
	const char* getSaveLocalDirectory()const { return ROOT; }

	const char* getBasePath();
	uint getFilesOnDir(const char* dir, std::vector<std::string>& files);
	uint getFilesOnDir(const char* dir, std::vector<std::string>& files, std::string extension);
	std::string getFileExtension(std::string file);
	unsigned int load(const char* file, char** buffer)const;
	SDL_RWops* load(const char* file)const;
	unsigned int save(const char* file, const char* buffer, unsigned int size)const;

	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);
};

#endif // !__MODULEFILESYSTEM_H__