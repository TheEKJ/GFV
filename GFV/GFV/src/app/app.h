#pragma once
#include "os/win/winadapter.h"
#include "viewer/viewer.h"

class App : public WindowAdapter
{
public:
	App();

	void Start(const wchar_t* filePath);

	void OnCreate();
	void OnPaint();
	void OnCommandExecute(const UINT& id);
	void OnDropFile(const wchar_t* filePath) { Start(filePath); };
	void OnResize();
private:
	Viewer m_viewer;
};