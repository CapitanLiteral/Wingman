#pragma once



enum ComponentType
{
	MESH,
	MATERIAL
};


class Component
{
public:
	bool enabled = true;

	Component(){}
	virtual ~Component(){}

	ComponentType type;

	virtual void enable() {}
	virtual void update() {}
	virtual void disable() {}
};

