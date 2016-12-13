#pragma once

enum game_resource_type
{
	RESOURCE_MESH,
	RESOURCE_TEXTURE
};

class GameResource
{
public:
	GameResource() {}
	virtual ~GameResource() {}

	bool active = false;
	uint32_t UUID;

private:

};