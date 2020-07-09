#pragma once
#include "os/win/winadapter.h"

class App : public WindowAdapter
{
public:
	App();

	void OnCreate();
};