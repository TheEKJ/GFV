#include "app/app.h"

#include <iostream>
#include <thread>

using namespace std;

int main()
{
	App window;
	/*
	POINT p;
	thread t([&]()->void
		{
			while (true)
			{
				system("cls");
				GetCursorPos(&p);
				//ScreenToClient(window.GetWindowHandle(), &p);
				cout << p.x << " : " << p.y << endl;
				cout << window.GetBounds().x << " : " << window.GetBounds().y << " : " << window.GetBounds().weidth << " : " << window.GetBounds().height << endl;
			}
		});
	t.detach();*/

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}