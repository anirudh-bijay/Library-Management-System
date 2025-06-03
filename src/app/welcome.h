#pragma once

#include "../utils/terminal_colours.h"

#include <iostream>

/// @brief Welcome page. Attempts to clear the console, then displays the
///        application title: “Library Management System”.
/// 
///        This is a stateless component.
class WelcomePage
{
    private:
        // This page is stateless.

    public:
        WelcomePage(bool build = true);
        ~WelcomePage() = default;

        void build() const;
};

/// @brief Default constructor. Builds the page by default, unless
///        @c `build` is set to `false`.
/// @param build `true` if the page should be built immediately; `false`
///        otherwise. `true` by default.
inline WelcomePage::WelcomePage(const bool build)
{
    if (build)
        this->build();
}

/// @brief Build the page.
inline void WelcomePage::build() const
{
    std::cout <<
        "\033c\n"         // ANSI escape code to (hopefully) clear the screen.
        "----------------------------------------\n"
        "       Library Management System        \n"
        "----------------------------------------\n"
        "                                        \n";
}
