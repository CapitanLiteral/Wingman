#pragma once

class GameObject;
class Outliner
{
public:
	Outliner();
	virtual ~Outliner();

	void draw();
	void treeNodes(GameObject* node);

	bool active;
};

