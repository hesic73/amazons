#include "core.hpp"
#include <iostream>

int main()
{
	auto game = Amazons(10);
	auto board = game.string_repr();
	std::cout << board << std::endl;
	return 0;
}
