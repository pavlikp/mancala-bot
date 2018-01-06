#include "BoardState.h"

BoardState::BoardState(bool moving_player, uint32_t cell1, uint32_t cell2, uint32_t cell3, uint32_t cell4,
	uint32_t cell5, uint32_t cell6, uint32_t cell7, uint32_t cell8, uint32_t cell9,
	uint32_t cell10, uint32_t cell11, uint32_t cell12, uint32_t cell13, uint32_t cell14)
	:
	m_moving_player(moving_player),
	m_cells{ cell1, cell2, cell3, cell4, cell5, cell6, cell7,
	cell8, cell9, cell10, cell11, cell12, cell13, cell14 }
{};

std::vector<uint32_t> BoardState::getMoves() const {
	std::vector<uint32_t> moves;
	uint32_t start = 0;
	if (!m_moving_player) {
		start = 7;
	}
	for (uint32_t i = start; i < start + 6; i++) {
		if (m_cells[i] > 0) {
			moves.push_back(i);
		}
	}
	return moves;
}

BoardState BoardState::makeMove(uint32_t cell) const {
	BoardState new_bs(*this);
	uint32_t cell_index = cell;
	// pass rocks counter-clockwise
	uint32_t empty = 0;
	while (new_bs.m_cells[cell] > empty) {
		if (++cell_index > 13) {
			cell_index -= 14;
		}
		if (cell_index == cell) {
			empty++;
		}
		if ((m_moving_player && cell_index != 13) || (!m_moving_player && cell_index != 6)) {
			new_bs.m_cells[cell_index]++;
			new_bs.m_cells[cell]--;
		}
	}
	// take opposing rocks if the last rock was passed to an empty cell
	if (new_bs.m_cells[cell_index] == 1 && cell_index != 6 && cell_index != 13) {
		if (m_moving_player && cell_index < 6) {
			new_bs.m_cells[6] += new_bs.m_cells[12 - cell_index] + 1;
			new_bs.m_cells[cell_index] = 0;
			new_bs.m_cells[12 - cell_index] = 0;
		}
		else if (!m_moving_player && cell_index > 6) {
			new_bs.m_cells[13] += new_bs.m_cells[12 - cell_index] + 1;
			new_bs.m_cells[cell_index] = 0;
			new_bs.m_cells[12 - cell_index] = 0;
		}
	}
	// other player's turn, except if the last rock landed in goal
	if (!(cell_index == 6 && m_moving_player) && !(cell_index == 13 && !m_moving_player)) {
		new_bs.m_moving_player = !m_moving_player;
	}
	// if no other moves are possible for the next turn, end game
	if (new_bs.getMoves().size() == 0) {
		if (new_bs.m_moving_player) {
			for (uint32_t i = 7; i < 13; i++) {
				new_bs.m_cells[13] += new_bs.m_cells[i];
				new_bs.m_cells[i] = 0;
			}
		}
		else {
			for (uint32_t i = 0; i < 6; i++) {
				new_bs.m_cells[6] += new_bs.m_cells[i];
				new_bs.m_cells[i] = 0;
			}
		}
	}
	return new_bs;
}

bool BoardState::getMovingPlayer() const {
	return m_moving_player;
}

int32_t BoardState::getMinMaxVal() const {
	return m_cells[13] - m_cells[6];
}

std::pair<int32_t, uint32_t> BoardState::MinMax(uint32_t depth, int32_t alpha, int32_t beta, std::chrono::steady_clock::time_point deadline) const {
	const auto moves = getMoves();
	if (moves.size() == 0 || depth == 0 || std::chrono::steady_clock::now() >= deadline) {
		if (getMovingPlayer()) { // give slightly better score to board states where it is the player's turn
			return std::pair<int32_t, uint32_t>(getMinMaxVal() - 1, 0);
		}
		else {
			return std::pair<int32_t, uint32_t>(getMinMaxVal(), 0);
		}
	}

	if (getMovingPlayer()) { // minimizing player
		int32_t best_val = INT32_MAX;
		uint32_t best_child;
		for (auto c : moves) {
			int32_t child_val = makeMove(c).MinMax(depth - 1, alpha, beta, deadline).first;
			if (child_val < best_val) {
				best_child = c;
				best_val = child_val;
			}
			beta = child_val < beta ? child_val : beta;
			if (beta <= alpha) {
				break;
			}
		}
		return std::pair<int32_t, uint32_t>(best_val, best_child);
	}
	else { // maximizing player
		int32_t best_val = INT32_MIN;
		uint32_t best_child;
		for (auto c : moves) {
			int32_t child_val = makeMove(c).MinMax(depth - 1, alpha, beta, deadline).first;
			if (child_val > best_val) {
				best_child = c;
				best_val = child_val;
			}
			alpha = child_val > alpha ? child_val : alpha;
			if (beta <= alpha) {
				break;
			}
		}
		return std::pair<int32_t, uint32_t>(best_val, best_child);
	}
}

uint32_t BoardState::MinMaxIterativeDeepening(std::chrono::steady_clock::time_point deadline) const {
	uint32_t depth = 5; // lowest posible time is 1s, hopefully any modern hardware can fully explore up to depth 5 in that time easily
	std::pair<int32_t, uint32_t> best_move;
	while (true) {
		auto best_move_current = MinMax(depth, INT32_MIN, INT32_MAX, deadline);
		if (std::chrono::steady_clock::now() < deadline) {
			best_move = best_move_current;
		}
		else {
			if ((getMovingPlayer() && best_move_current.first <= best_move.first) ||
				(!getMovingPlayer() && best_move_current.first >= best_move.first)) {
				best_move = best_move_current;
			}
			break;
		}
		depth++;
	}
	return best_move.second;
}

std::ostream& operator<<(std::ostream& lhs, const BoardState& rhs) {
	for (uint32_t i = 13; i > 6; i--) {
		lhs.width(2);
		lhs << rhs.m_cells[i] << "|";
	}
	if (!rhs.m_moving_player) {
		lhs << "  MOVING";
	}
	lhs << std::endl << " ";
	for (uint32_t i = 0; i < 7; i++) {
		lhs.width(2);
		lhs << "|" << rhs.m_cells[i];
	}
	if (rhs.m_moving_player) {
		lhs << "  MOVING";
	}
	return lhs << std::endl;
}