#pragma once

class WindowMenus
{
public:
	WindowMenus();
	~WindowMenus();

	void draw();
	void showCredits();
	void importFBX();
	void loadPrefab();

	bool quitSelected = false;
	bool aboutSelected = false;
	bool creditsSelected = false;
	bool openReleaseDirectory = false;
	bool openRepoDirectory = false;
	bool showfpsInfo = false;
	bool showImportFBX = false;
	bool showLoadPrefab = false;

	bool QUIT = false;


	bool credits = false;
};

