#include <iostream>
#include <cstdint>
#include <array>
#include <cstring>
#include "core.h"

static constexpr void set_cell(int *board, std::uint32_t size, std::uint32_t x, std::uint32_t y, Cell c)
{
	board[x * size + y] = static_cast<int>(c);
}
static constexpr Cell get_cell(const int *board, std::uint32_t size, std::uint32_t x, std::uint32_t y)
{
	return static_cast<Cell>(board[x * size + y]);
}
static constexpr bool is_valid_position(std::uint32_t size, std::uint32_t x, std::uint32_t y)
{
	return (x < size) && (y < size);
}

static constexpr bool is_empty(const int *board, std::uint32_t size, std::uint32_t x, std::uint32_t y)
{
	return is_valid_position(size, x, y) && get_cell(board, size, x, y) == Cell::Empty;
}
static bool piece_no_liberties(const int *board, std::uint32_t size, std::uint32_t x, std::uint32_t y)
{
	if (is_empty(board, size, x - 1, y))
		return false;
	if (is_empty(board, size, x - 1, y - 1))
		return false;
	if (is_empty(board, size, x - 1, y + 1))
		return false;
	if (is_empty(board, size, x, y - 1))
		return false;
	if (is_empty(board, size, x, y + 1))
		return false;
	if (is_empty(board, size, x + 1, y))
		return false;
	if (is_empty(board, size, x + 1, y - 1))
		return false;
	if (is_empty(board, size, x + 1, y + 1))
		return false;
	return true;
}

Amazons::Amazons(std::uint32_t size) : size(size), num_cells(size * size)
{
	board = new int[num_cells];
	reset();
}

Amazons::~Amazons()
{
	delete[] board;
}

void Amazons::reset()
{
	std::memset(board, 0, sizeof(int) * num_cells);

	auto offset = (size - 1) / 3;
	set_cell(board, size, offset, 0, Cell::Black);
	set_cell(board, size, 0, offset, Cell::Black);
	set_cell(board, size, size - 1 - offset, 0, Cell::Black);
	set_cell(board, size, size - 1, offset, Cell::Black);
	black_positions[0][0] = offset;
	black_positions[0][1] = 0;
	black_positions[1][0] = 0;
	black_positions[1][1] = offset;
	black_positions[2][0] = size - 1 - offset;
	black_positions[2][1] = 0;
	black_positions[3][0] = size - 1;
	black_positions[3][1] = offset;

	set_cell(board, size, 0, size - 1 - offset, Cell::White);
	set_cell(board, size, offset, size - 1, Cell::White);
	set_cell(board, size, size - 1 - offset, size - 1, Cell::White);
	set_cell(board, size, size - 1, size - 1 - offset, Cell::White);
	white_positions[0][0] = 0;
	white_positions[0][1] = size - 1 - offset;
	white_positions[1][0] = offset;
	white_positions[1][1] = size - 1;
	white_positions[2][0] = size - 1 - offset;
	white_positions[2][1] = size - 1;
	white_positions[3][0] = size - 1;
	white_positions[3][1] = size - 1 - offset;

	turn = Cell::Black;
	move_count = 0;
	done = false;
}

bool Amazons::is_valid_action(std::uint32_t x0, std::uint32_t y0, std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2)
{
	if (!(is_valid_position(size, x0, y0) &&
		  is_valid_position(size, x1, y1) &&
		  is_valid_position(size, x2, y2)))
	{
		return false;
	}
	if (get_cell(board, size, x0, y0) != turn)
	{
		return false;
	}

	if (get_cell(board, size, x1, y1) != Cell::Empty)
	{
		return false;
	}

	if (get_cell(board, size, x2, y2) != Cell::Empty && !(x0 == x2 && y0 == y2))
	{
		return false;
	}

	return true;
}

bool Amazons::step(std::uint32_t x0, std::uint32_t y0, std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2)
{
	if (done)
	{
		return false;
	}
	if (!is_valid_action(x0, y0, x1, y1, x2, y2))
	{
		return false;
	}

	set_cell(board, size, x0, y0, Cell::Empty);
	set_cell(board, size, x1, y1, turn);
	set_cell(board, size, x2, y2, Cell::Arrow);

	if (turn == Cell::Black)
	{
		for (int i = 0; i < 4; i++)
		{
			if (black_positions[i][0] == x0 && black_positions[i][1] == y0)
			{
				black_positions[i][0] = x1;
				black_positions[i][1] = y1;
				break;
			}
		}
		turn = Cell::White;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (white_positions[i][0] == x0 && white_positions[i][1] == y0)
			{
				white_positions[i][0] = x1;
				white_positions[i][1] = y1;
				break;
			}
		}
		turn = Cell::Black;
	}

	move_count += 1;
	if (turn == Cell::Black)
	{
		done = piece_no_liberties(board, size, black_positions[0][0], black_positions[0][1]) &&
			   piece_no_liberties(board, size, black_positions[1][0], black_positions[1][1]) &&
			   piece_no_liberties(board, size, black_positions[2][0], black_positions[2][1]) &&
			   piece_no_liberties(board, size, black_positions[3][0], black_positions[3][1]);
	}
	else
	{
		done = piece_no_liberties(board, size, white_positions[0][0], white_positions[0][1]) &&
			   piece_no_liberties(board, size, white_positions[1][0], white_positions[1][1]) &&
			   piece_no_liberties(board, size, white_positions[2][0], white_positions[2][1]) &&
			   piece_no_liberties(board, size, white_positions[3][0], white_positions[3][1]);
	}
	return true;
}

std::string Amazons::string_repr()
{
	std::string s;
	s.reserve(num_cells + size);
	for (std::uint32_t i = 0; i < size; i++)
	{
		for (std::uint32_t j = 0; j < size; j++)
		{
			char c;

			switch (get_cell(board, size, j, i))
			{
			case Cell::Empty:
				c = ' ';
				break;
			case Cell::Black:
				c = 'B';
				break;
			case Cell::White:
				c = 'W';
				break;
			case Cell::Arrow:
				c = 'A';
				break;
			default:
				c = '?';
			}
			s.push_back(c);
		}
		s.push_back('\n');
	}
	return s;
}