#include "game_result.hpp"

#include <format>
#include <stdexcept>

#include "player.hpp"

namespace othellite::game {


std::string WinByScore::to_string() const
{
    return std::format(
        "{} ({}) won.\nThe score was {}.",
        get_winner().get_name(),
        player_color_to_string(get_winner().get_color()),
        get_score().to_string(get_winner()));
}

std::string WinByOpponentMistake::to_string() const
{
    return std::format(
        "{} ({}) won.\nThe opponent made an invalid move.",
        get_winner().get_name(),
        player_color_to_string(get_winner().get_color()));
}

std::string TiedResult::to_string() const
{
    return std::format(
        "The game was a tie.\nThe score was {}.",
        get_score().to_string(PlayerColor::dark));
}

} // namespace othellite::game