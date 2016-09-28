#include "ModuleFBXImporter.h"
#include "Assimp\Assimp\include\scene.h"
#include "Assimp\Assimp\include\cfileio.h"
#include "Assimp\Assimp\include\cimport.h"
#include "Assimp\Assimp\include\postprocess.h"

#pragma comment (lib, "Assimp\Assimp\libx86\assimp.lib")


ModuleFBXImporter::ModuleFBXImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleFBXImporter::~ModuleFBXImporter()
{
}
