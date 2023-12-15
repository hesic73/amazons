#pragma once
#include <iostream>
#include <cstdint>
#include <array>
#include <cstring>

enum Cell
{
	Empty = 0, // a must
	Black = 1,
	White = 2,
	Arrow = 3,
};


class Amazons
{
public:
	Amazons(std::uint32_t size);
	~Amazons();
	void reset();
	bool step(std::uint32_t x0, std::uint32_t y0, std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2);
	const std::uint32_t size;
	bool is_done() { return done; }
	const int *get_board_view() { return board; }
	// debug
	std::string string_repr();

private:
	const std::uint32_t num_cells;
	int *board;
	Cell turn;
	std::uint32_t move_count;
	bool done;
	std::array<std::array<std::uint32_t, 2>, 4> black_positions;
	std::array<std::array<std::uint32_t, 2>, 4> white_positions;

	bool is_valid_action(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
};
