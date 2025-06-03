#pragma once

#include "../objects/users.h"
#include "../utils/terminal_colours.h"
#include "app.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

class ChangePasswordPage
{ 
    private:       
        /// @brief App state handle.
        struct
        {
            const bool& is_logged_in;
            lms::User& logged_in_user;
        } app_state;

    public:
        ChangePasswordPage(App::State& app_state, bool build = true);
        ~ChangePasswordPage() = default;

        void build();
};

inline ChangePasswordPage::ChangePasswordPage(App::State& app_state, const bool build)
    : app_state{
        .is_logged_in   = app_state.is_logged_in,
        .logged_in_user = app_state.logged_in_user,
    }
{
    if (build)
        this->build();
}

inline void ChangePasswordPage::build()
{
    // Mainloop
    while (true) {
        if (!app_state.is_logged_in)
            throw std::logic_error("Unauthorised access!");

        std::cout <<
            "\033c\n"         // ANSI escape code to (hopefully) clear the screen.
            "----------------------------------------\n"
            "            Change Password             \n"
            "----------------------------------------\n"
            "                                        \n"
            " Enter new password: ";

        std::string password;
        std::getline(std::cin, password);

        if (std::find(password.cbegin(), password.cend(), '\n') != password.cend()) {
            std::cout << ANSI_COLOR_RED << "\nNewlines not allowed in password\n" << ANSI_COLOR_RESET;
            std::cout << "Press any key to continue...";
            std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        } else {
            app_state.logged_in_user = lms::User{app_state.logged_in_user.getUserId(), password};
            std::cout << ANSI_COLOR_GREEN << "\nRequest registered\n" << ANSI_COLOR_RESET;
            std::cout << "Password change shall take effect on logout.\n";
            std::cout << "Press enter to continue...";
            std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
            std::cout << "\033c\n"; // ANSI escape code to (hopefully) clear the screen.
            break;
        }
    }
}
