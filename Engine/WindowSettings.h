#pragma once
#include "UI_Item.h"


class WindowSettings :	public UI_Item
{
public:
	WindowSettings();
	virtual ~WindowSettings();


	bool showSettings;

	void draw();

};

