#include "stdafx.h"

#include <iostream>

#include "GameData/Application.h"

int main() {
	try
	{
		Application App;
		App.Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << "What occurred: " << ex.what();
	}
	return 0;
}
