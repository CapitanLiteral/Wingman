#pragma once
#include <vector>

#include "Module.h"


class GameObject;
struct aiScene;
struct aiNode;

class GameObjectManager :
	public Module
{
public:
	GameObjectManager(Application* app, bool start_enabled = true);
	virtual ~GameObjectManager();

	bool Start();

	void LoadScene(const aiScene* scene, const aiNode* node, GameObject* parent);

	std::vector<GameObject*> entities;

};

