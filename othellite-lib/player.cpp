#include "player.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "board.hpp"


namespace othellite::game {

void Player::start_game(PlayerColor const pc) { color = pc; }

void Player::game_over(GameResult const& result) {}

auto RandomPlayer::pick_move(Board const& board) const -> grid::Position
{
    auto const moves = board.find_valid_moves(get_color());

    std::vector<grid::Position> out{};
    auto gen = std::mt19937{std::random_device{}()};
    std::ranges::sample(moves, std::back_inserter(out), 1, gen);

    assert(!out.empty());
    return out[0];
}
} // namespace othellite::game