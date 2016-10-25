#pragma once


class WindowMenus
{
public:
	WindowMenus();
	~WindowMenus();

	void draw();

	bool quitSelected = false;
	bool aboutSelected = false;
	bool openReleaseDirectory = false;
	bool openRepoDirectory = false;
	bool showfpsInfo = false;

	bool QUIT = false;
};

