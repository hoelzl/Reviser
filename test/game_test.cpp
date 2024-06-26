// Copyright (c) 2021-2024 Dr. Matthias Hölzl.

#include "game.hpp"

#include <string>

#include "array_board.hpp"
#include "default_game.hpp"
#include "doctest.hpp"
#include "game_result.hpp"
#include "utilities.hpp"

namespace reviser {

using namespace std::string_literals;

TEST_CASE("Notifier")
{
    auto notifier = SpyForNotifierOutput{};

    SUBCASE("display_board()")
    {
        const auto board_string = "|*|O|O|O| | | | |\n"
                                  "| |*| | | | | | |\n"
                                  "| | | |O| | | | |\n"
                                  "| | | |O|O| | | |\n"
                                  "| | | |O|*| | | |\n"
                                  "| | | | | | | | |\n"
                                  "| |*|O|*|O|*|O| |\n"
                                  "| | |*|O|*|O| | |"s;
        const auto board = ArrayBoard::from_string(board_string);

        notifier.display_board(board);
        CHECK(notifier.output() == board_string + "\n"s);
    }

    SUBCASE("note_new_game()")
    {
        auto dark_player
            = std::make_shared<ConstantPlayerStub>("dark_player", PlayerColor::dark);
        auto light_player
            = std::make_shared<ConstantPlayerStub>("light_player", PlayerColor::light);
        auto players = Players{dark_player, light_player};

        auto empty_board_string = "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"
                                  "| | | | | | | | |\n"s;
        auto constexpr board = ArrayBoard{};
        std::string expected
            = ("Starting a new game.\n"s + "Dark player: dark_player\n"s
               + "Light player: light_player\n"s + empty_board_string);

        notifier.note_new_game(players, board);

        CHECK(notifier.output() == expected);
    }

    SUBCASE("note_new_game()")
    {
        auto dark_player
            = std::make_shared<ConstantPlayerStub>("dark_player", PlayerColor::dark);

        auto board_string = "|*|O|O|O| | | | |\n"
                            "| |*| | | | | | |\n"
                            "| | | | | | | | |\n"
                            "| | | |*|O| | | |\n"
                            "| | | |O|*| | | |\n"
                            "| | | | | | | | |\n"
                            "| | | | | | | | |\n"
                            "| | | | | | | | |"s;
        auto board = ArrayBoard::from_string(board_string);
        std::string expected
            = ("\ndark_player (dark) plays (2, 3).\n"s + board_string + "\n"s);

        notifier.note_move(*dark_player, Position{Row{1}, Column{2}}, board);

        CHECK(notifier.output().size() == expected.size());
        CHECK(notifier.output() == expected);
    }

    SUBCASE("note_result()")
    {
        auto dark_player
            = std::make_shared<ConstantPlayerStub>("dark_player", PlayerColor::dark);
        auto light_player
            = std::make_shared<ConstantPlayerStub>("light_player", PlayerColor::light);
        auto board = ArrayBoard{};

        SUBCASE("Dark player won with best score.")
        {
            auto score = Score{44, 20, 0};
            auto result = WinByScore{score, board, *dark_player, *light_player};
            auto expected
                = "\nGAME OVER.\ndark_player (dark) won.\nThe score was 44:20.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("White player won with best score.")
        {
            auto score = Score{24, 40, 0};
            auto result = WinByScore{score, board, *light_player, *dark_player};
            auto expected
                = "\nGAME OVER.\nlight_player (light) won.\nThe score was 40:24.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("Dark player won because of error by light player.")
        {
            auto score = Score{24, 40, 0};
            auto result
                = WinByOpponentMistake{score, board, *dark_player, *light_player};
            auto expected
                = "\nGAME OVER.\ndark_player (dark) won.\nThe opponent made an "
                  "invalid move.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("Tie.")
        {
            auto score = Score{32, 32, 0};
            auto result = TiedResult{score, board, *dark_player, *light_player};
            auto expected
                = "\nGAME OVER.\nThe game was a tie.\nThe score was 32:32.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }
    }
}

TEST_CASE("Test game for the minimal player.")
{
    auto dark_player
        = std::make_shared<MinimalPlayer>("dark_player", PlayerColor::dark);
    auto light_player
        = std::make_shared<MinimalPlayer>("light_player", PlayerColor::light);
    auto notifier_spy = std::make_unique<NotifierSpy>();
    // We rely on the game keeping the notifier spy alive for us...
    const auto* notifier_spy_ptr = notifier_spy.get();
    auto game = std::make_unique<DefaultGame<ArrayBoard>>(
        dark_player, light_player, std::move(notifier_spy));

    const auto expected_moves = std::vector<NotifierSpy::Move>{
        {"dark_player", PlayerColor::dark, 2, 4},
        {"light_player", PlayerColor::light, 2, 3},
        {"dark_player", PlayerColor::dark, 1, 2},
        {"light_player", PlayerColor::light, 1, 3},
        {"dark_player", PlayerColor::dark, 0, 1},
        {"light_player", PlayerColor::light, 0, 3},
        {"dark_player", PlayerColor::dark, 0, 2},
        {"light_player", PlayerColor::light, 0, 0},
        {"dark_player", PlayerColor::dark, 2, 2},
        {"light_player", PlayerColor::light, 2, 1},
        {"dark_player", PlayerColor::dark, 1, 1},
        {"light_player", PlayerColor::light, 2, 0},
        {"dark_player", PlayerColor::dark, 1, 0},
        {"light_player", PlayerColor::light, 2, 5},
        {"dark_player", PlayerColor::dark, 1, 4},
        {"light_player", PlayerColor::light, 0, 4},
        {"dark_player", PlayerColor::dark, 1, 5},
        {"light_player", PlayerColor::light, 0, 5},
        {"dark_player", PlayerColor::dark, 0, 6},
        {"light_player", PlayerColor::light, 0, 7},
        {"dark_player", PlayerColor::dark, 1, 6},
        {"light_player", PlayerColor::light, 2, 6},
        {"dark_player", PlayerColor::dark, 1, 7},
        {"light_player", PlayerColor::light, 2, 7},
        {"dark_player", PlayerColor::dark, 3, 0},
        {"light_player", PlayerColor::light, 3, 1},
        {"dark_player", PlayerColor::dark, 3, 2},
        {"light_player", PlayerColor::light, 3, 5},
        {"dark_player", PlayerColor::dark, 3, 6},
        {"light_player", PlayerColor::light, 4, 0},
        {"dark_player", PlayerColor::dark, 4, 1},
        {"light_player", PlayerColor::light, 3, 7},
        {"dark_player", PlayerColor::dark, 4, 2},
        {"light_player", PlayerColor::light, 4, 5},
        {"dark_player", PlayerColor::dark, 4, 7},
        {"light_player", PlayerColor::light, 4, 6},
        {"dark_player", PlayerColor::dark, 5, 0},
        {"light_player", PlayerColor::light, 5, 1},
        {"dark_player", PlayerColor::dark, 5, 2},
        {"light_player", PlayerColor::light, 5, 3},
        {"dark_player", PlayerColor::dark, 5, 4},
        {"light_player", PlayerColor::light, 5, 5},
        {"dark_player", PlayerColor::dark, 5, 6},
        {"light_player", PlayerColor::light, 5, 7},
        {"dark_player", PlayerColor::dark, 6, 2},
        {"light_player", PlayerColor::light, 6, 0},
        {"dark_player", PlayerColor::dark, 6, 4},
        {"light_player", PlayerColor::light, 6, 1},
        {"dark_player", PlayerColor::dark, 6, 5},
        {"light_player", PlayerColor::light, 6, 3},
        {"dark_player", PlayerColor::dark, 7, 1},
        {"light_player", PlayerColor::light, 6, 6},
        {"dark_player", PlayerColor::dark, 7, 2},
        {"light_player", PlayerColor::light, 6, 7},
        {"dark_player", PlayerColor::dark, 7, 0},
        {"light_player", PlayerColor::light, 7, 3},
        {"dark_player", PlayerColor::dark, 7, 4},
        {"light_player", PlayerColor::light, 7, 5},
        {"dark_player", PlayerColor::dark, 7, 6},
        {"light_player", PlayerColor::light, 7, 7},
    };

    game->new_game(false);
    game->run_game_loop();

    for (auto i = 0u; i < expected_moves.size(); ++i) {
        CHECK(notifier_spy_ptr->moves[i] == expected_moves[i]);
    }

    CHECK(notifier_spy_ptr->result_summary.type == "win"s);
    CHECK(notifier_spy_ptr->result_summary.winner == PlayerColor::light);
    CHECK(notifier_spy_ptr->result_summary.loser == PlayerColor::dark);
}

} // namespace reviser
