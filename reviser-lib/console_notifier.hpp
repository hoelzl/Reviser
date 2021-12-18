// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once

#ifndef REVISER_LIB_CONSOLE_NOTIFIER_HPP
#define REVISER_LIB_CONSOLE_NOTIFIER_HPP

#include "game.hpp"

namespace reviser::game {

class ConsoleNotifier final : public Notifier
{
public:
    ConsoleNotifier() = default;
    void display_message(std::string_view message) override;
};
} // namespace reviser::game

#endif
