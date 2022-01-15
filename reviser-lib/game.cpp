// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "game.hpp"

#include <format>
#include <string>

#include "array_board.hpp"
#include "game_result.hpp"


namespace reviser::game {

using namespace std::string_literals;

void Players::swap_dark_and_light_player()
{
    swap(dark_player, light_player);
    dark_player.get().set_color(PlayerColor::dark);
    light_player.get().set_color(PlayerColor::light);
}

void Players::new_game() const
{
    get_dark_player().new_game();
    get_light_player().new_game();
}

void Notifier::display_board(const ArrayBoard& board) { display_message(board.to_string()); }

void Notifier::note_new_game(const Players& players, const ArrayBoard& board)
{
    display_message("Starting a new game.");

    auto message = std::format("Dark player: {}", players.get_dark_player().get_name());
    display_message(message);

    message = std::format("Light player: {}", players.get_light_player().get_name());
    display_message(message);
    display_board(board);
}

void Notifier::note_move(
    const Player& player, const grid::Position pos, const ArrayBoard& board)
{
    const auto message = std::format(
        "\n{} ({}) plays {}.",
        player.get_name(),
        player_color_to_string(player.get_color()),
        pos.to_string());

    display_message(message);
    display_board(board);
}

void Notifier::note_result(const GameResult& result)
{
    const auto message = "\nGAME OVER.\n"s + result.to_string();
    display_message(message);
}

} // namespace reviser::game
