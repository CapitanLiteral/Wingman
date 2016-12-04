#pragma once

class WindowMenus
{
public:
	WindowMenus();
	~WindowMenus();

	void draw();
	void showCredits();

	bool quitSelected = false;
	bool aboutSelected = false;
	bool creditsSelected = false;
	bool openReleaseDirectory = false;
	bool openRepoDirectory = false;
	bool showfpsInfo = false;

	bool QUIT = false;


	bool credits = false;
};

