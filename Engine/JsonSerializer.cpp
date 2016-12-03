#include "JsonSerializer.h"
#include "IJsonSerializable.h"
#include "ModuleFileSystem.h"

#include <json\json.h>


bool JsonSerializer::Serialize(IJsonSerializable * pObj, std::string & output, std::string path)
{
	if (pObj == NULL)
		return false;

	Json::Value serializeRoot;
	pObj->Serialize(serializeRoot);

	Json::StyledWriter writer;
	output = writer.write(serializeRoot);
	App->fs->save(path.c_str(), output.c_str(), output.size());

	return true;
}

bool JsonSerializer::Deserialize(IJsonSerializable * pObj, std::string & input)
{
	if (pObj == NULL)
		return false;

	Json::Value deserializeRoot;
	Json::Reader reader;

	if (!reader.parse(input, deserializeRoot))
		return false;

	pObj->Deserialize(deserializeRoot);

	return true;
}

bool JsonSerializer::Deserialize(IJsonSerializable * pObj, std::string path)
{
	if (pObj == NULL)
		return false;

	char* buffer;
	App->fs->load(path.c_str(), &buffer);
	std::string input(buffer);
	RELEASE_ARRAY(buffer);
	Json::Value deserializeRoot;
	Json::Reader reader;

	if (!reader.parse(input, deserializeRoot))
		return false;

	pObj->Deserialize(deserializeRoot);

	return true;
}
