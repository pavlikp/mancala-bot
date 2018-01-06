#pragma once

#include <vector>
#include <ostream>
#include <chrono>

class BoardState {
public:
	BoardState(bool moving_player, uint32_t cell1, uint32_t cell2, uint32_t cell3, uint32_t cell4,
		uint32_t cell5, uint32_t cell6, uint32_t cell7, uint32_t cell8, uint32_t cell9,
		uint32_t cell10, uint32_t cell11, uint32_t cell12, uint32_t cell13, uint32_t cell14);

	/**
	* Returns a pair of best possible move by MinMax and its value (value, move index)
	*/
	uint32_t MinMaxIterativeDeepening(std::chrono::steady_clock::time_point deadline) const;

private:
	bool m_moving_player; // true - possible moves 0-5, goal 6; false - possible moves 7-12, goal 13
	uint32_t m_cells[14];

	/**
	* Prints board to selected output stream in readable format. // not used
	*/
	friend std::ostream& operator<<(std::ostream& lhs, const BoardState& rhs);

	/**
	* MinMax with AlphaBeta cutoff.
	*/
	std::pair<int32_t, uint32_t> MinMax(uint32_t depth, int32_t alpha, int32_t beta, std::chrono::steady_clock::time_point deadline) const;

	/**
	* Returns possible moves for the currently turning player
	*/
	std::vector<uint32_t> getMoves() const;

	/**
	* Returns board state after move specified by cell index
	*/
	BoardState makeMove(uint32_t cell) const;

	/**
	* Returns moving player (true - possible moves 0-5, goal 6; false - possible moves 7-12, goal 13)
	*/
	bool getMovingPlayer() const;

	/**
	* Returns value of heuristic for the board state
	*/
	int32_t getMinMaxVal() const;
};