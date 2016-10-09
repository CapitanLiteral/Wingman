#pragma once



enum ComponentType
{
	MESH,
	TRANSFORM,
	MATERIAL
};


class Component
{
public:
	Component(){}
	virtual ~Component(){}

	ComponentType type;

	virtual void enable() {}
	virtual void update() {}
	virtual void disable() {}
};

