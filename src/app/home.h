#pragma once

#include "../services/accounts.h"
#include "../services/books.h"
#include "../services/users.h"
#include "../utils/terminal_colours.h"
#include "app.h"

#include <iostream>
#include <limits>

template <App::State::UserType UserRole>
class HomePage;

#pragma region      Librarian homepage

template <>
class HomePage <App::State::UserType::LIBRARIAN>
{
    public:
        /// @brief Enums to represent actions requested by the user through
        ///        the homepage.
        enum Actions : std::uint_least8_t {
            EDIT_BOOKS, MANAGE_USERS, CHANGE_PASSWORD, LOGOUT
        };

    private:
        /// @brief App state handle.
        struct
        {
            bool& is_logged_in;
            lms::User& logged_in_user;  // Non-const to enable password change.
            const App::State::UserType& user_type;
        } app_state;
        
        Actions& next_action;

    public:
        HomePage(App::State& app_state,
                //  services::Accounts& accounts_service,
                //  services::Books& books_service,
                //  services::Users& users_service,
                 Actions& next_action,
                 bool build = true);

        void build();
};

inline HomePage <App::State::UserType::LIBRARIAN>::HomePage(
    App::State& app_state,
    Actions& next_action,
    const bool build
)
    : app_state{
        .is_logged_in   = app_state.is_logged_in,
        .logged_in_user = app_state.logged_in_user,
        .user_type      = app_state.user_type
    },
    next_action{next_action}
{
    if (build)
        this->build();
}

inline void
HomePage <App::State::UserType::LIBRARIAN>::build()
{
    std::cout << "\033c";           // ANSI escape code to (hopefully) clear the screen.

    // Mainloop
    while (app_state.is_logged_in) {
        std::cout << '\n' <<
            "----------------------------------------\n"
            "                Actions                 \n"
            "----------------------------------------\n"
            "                                        \n"
            " 1. Edit book catalogue                 \n"
            " 2. Manage users                        \n"
            " 3. Change password                     \n"
            " 4. Logout                              \n"
            "                                        \n"
            " Select a choice (1-4): ";

        unsigned choice;            // Serial number of the action chosen.
        std::cin >> choice;
        // Clear input stream (`std::cin`).
        if (!std::cin)
            std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << '\n';

        switch (choice) {
            case 1:
                next_action = EDIT_BOOKS;
                return;

            case 2:
                next_action = MANAGE_USERS;
                return;

            case 3:
                next_action = CHANGE_PASSWORD;
                return;

            case 4:
                next_action = LOGOUT;
                return;
            
            default:
                std::cout <<
                    ANSI_COLOR_MAGENTA << "Invalid choice\n" << ANSI_COLOR_RESET;
        }
    }
}

#pragma endregion   Librarian homepage

#pragma region      Student/Faculty homepage

template <>
class HomePage <App::State::UserType::STUDENT>
{
    public:
        /// @brief Enums to represent actions requested by the user through
        ///        the homepage.
        enum Actions : std::uint_least8_t {
            SEARCH_BOOKS, BORROW, RETURN, PAYMENT, CHANGE_PASSWORD, LOGOUT
        };

    private:
        /// @brief App state handle.
        struct
        {
            bool& is_logged_in;
            lms::User& logged_in_user;  // Non-const to enable password change.
            const App::State::UserType& user_type;
            lms::Account& user_account;
        } app_state;
        
        Actions& next_action;

    public:
        HomePage(App::State& app_state,
                 Actions& next_action,
                 bool build = true);

        void build();
};

inline HomePage <App::State::UserType::STUDENT>::HomePage(
    App::State& app_state,
    Actions& next_action,
    const bool build
)
    : app_state{
        .is_logged_in   = app_state.is_logged_in,
        .logged_in_user = app_state.logged_in_user,
        .user_type      = app_state.user_type,
        .user_account   = app_state.user_account
    },
    next_action{next_action}
{
    if (build)
        this->build();
}

inline void HomePage <App::State::UserType::STUDENT>::build()
{
    std::cout << "\033c";           // ANSI escape code to (hopefully) clear the screen.

    // Mainloop
    while (app_state.is_logged_in) {
        std::cout << '\n' <<
            "----------------------------------------\n"
            "                Actions                 \n"
            "----------------------------------------\n"
            "                                        \n"
            " 1. Search book catalogue               \n"
            " 2. Borrow a book                       \n"
            " 3. Return a book                       \n"
            " 4. View and pay outstanding dues       \n"
            " 5. Change password                     \n"
            " 6. Logout                              \n"
            "                                        \n"
            " Select a choice (1-6): ";

        unsigned choice;            // Serial number of the action chosen.
        std::cin >> choice;
        if (!std::cin)
            std::cin.clear();
        // Clear input stream (`std::cin`).
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << '\n';

        switch (choice) {
            case 1:
                next_action = SEARCH_BOOKS;
                return;

            case 2:
                next_action = BORROW;
                return;

            case 3:
                next_action = RETURN;
                return;

            case 4:
                next_action = PAYMENT;
                return;

            case 5:
                next_action = CHANGE_PASSWORD;
                return;

            case 6:
                next_action = LOGOUT;
                return;
            
            default:
                std::cout <<
                    ANSI_COLOR_MAGENTA << "Invalid choice\n" << ANSI_COLOR_RESET;
        }
    }
}

#pragma endregion   Student/Faculty homepage