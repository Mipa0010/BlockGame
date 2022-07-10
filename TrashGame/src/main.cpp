#include <exception>
#include <iostream>
#include "TrashGame.h"

int main(int argc, char* argv[])
{
	try
	{
		auto trash = CreateGame();
		trash->Run();
		delete trash;
	}
	catch(const std::exception& ex)
	{
		std::cout << "Error: " << ex.what() << std::endl;
	}

	return (0);
}