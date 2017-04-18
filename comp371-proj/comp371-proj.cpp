// comp371-proj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestApp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TestApp app("World");
	app.initialize();
	app.run();
	return 0;
}

