#pragma once
#include <vector>
#include <map>

#include "Module.h"

class GameObject;
struct aiScene;
struct aiNode;

class GameObjectManager : public Module
{
public:
	GameObjectManager(Application* app, bool start_enabled = true);
	virtual ~GameObjectManager();


	bool Start();
	update_status Update(float dt);


	GameObject* LoadFBX(const char* path);
	void LoadScene(const aiScene* scene, const aiNode* node, GameObject* parent);

	GameObject* root = nullptr;

	GameObject* getFocusGO ();
	void setFocusGO(GameObject* focusGO);

	std::map<std::string, uint> textures;
	GameObject* currentCamera = nullptr;

	void saveScene();
	void loadScene();
	void loadPrefab(std::string name);
	bool haveToSaveScene = false;
	bool haveToLoadScene = false;

	std::vector<GameObject*> toDelete;

private:
	
	GameObject* focusGO = nullptr;

	

};

