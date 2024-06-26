// Copyright (c) 2021-2024 Dr. Matthias Hölzl.

#include "game.hpp"

#include <format>
#include <string>

#include "array_board.hpp"
#include "game_result.hpp"


namespace reviser {

using namespace std::string_literals;
using ::reviser::BasicBoard;

Players::Players(std::shared_ptr<Player> dp, std::shared_ptr<Player> lp)
    : dark_player{std::move(dp)}
    , light_player{std::move(lp)}
{
    // Make sure not to access the arguments in the body of the constructor, since
    // we have moved from them!
    if (dark_player == nullptr || light_player == nullptr) {
        throw std::invalid_argument("Must provide valid players.");
    }
    dark_player->set_color(PlayerColor::dark);
    light_player->set_color(PlayerColor::light);
}

const Player& Players::get_other_player(const Player& player) const
{
    if (player == get_light_player()) {
        return get_dark_player();
    }
    return get_light_player();
}

Player& Players::get_other_player(const Player& player)
{
    if (player == get_light_player()) {
        return get_dark_player();
    }
    return get_light_player();
}

void Players::swap_dark_and_light_player()
{
    swap(dark_player, light_player);
    get_dark_player().set_color(PlayerColor::dark);
    get_light_player().set_color(PlayerColor::light);
}

void Players::new_game()
{
    get_dark_player().new_game();
    get_light_player().new_game();
}

void Notifier::display_board(const BasicBoard& board)
{
    display_message(board.to_string());
}

void Notifier::note_new_game(const Players& players, const BasicBoard& board)
{
    display_message("Starting a new game.");

    auto message = std::format("Dark player: {}", players.get_dark_player().get_name());
    display_message(message);

    message = std::format("Light player: {}", players.get_light_player().get_name());
    display_message(message);
    display_board(board);
}

void Notifier::note_move(
    const Player& player, const Position pos, const BasicBoard& board)
{
    const auto message = std::format(
        "\n{} ({}) plays {}.",
        player.get_name(),
        player_color_to_string(player.get_color()),
        pos.to_user_string());

    display_message(message);
    display_board(board);
}

void Notifier::note_result(const GameResult& result)
{
    const auto message = "\nGAME OVER.\n"s + result.to_string();
    display_message(message);
}

} // namespace reviser
