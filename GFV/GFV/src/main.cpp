#include "app/app.h"

#include <iostream>
#include <thread>

using namespace std;

int main()
{
	App window;
	assert(window.init());
	/*
	POINT p;
	thread t([&]()->void
		{
			once_flag flag;

			while (true)
			{
				//system("cls");
				call_once(flag, []()->void {cout << "Call once" << endl; });
				GetCursorPos(&p);
				//ScreenToClient(window.GetWindowHandle(), &p);
				cout << p.x << " : " << p.y << endl;
				cout << window.GetBounds().x << " : " << window.GetBounds().y << " : " << window.GetBounds().weidth << " : " << window.GetBounds().height << endl;
				Sleep(5000);
			}
		});
	t.detach();*/

	CoInitialize(nullptr);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	CoUninitialize();
	return 0;
}