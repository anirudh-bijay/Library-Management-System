#pragma once

#include "../objects/users.h"
#include "../services/users.h"
#include "app.h"

#include <string>

class LoginPage
{ 
    public:
        // Maximum number of password attempts before program terminates.
        static constexpr unsigned int max_password_attempts = 5;

        /// @brief Page state. POD `struct`.
        struct State
        {
            std::string username;   // Username from user input. State variable.
            std::string password;   // Password from user input. State variable.
            services::Users::row_type user_row;
        };

    private:
        State state;                /** @brief Page state. */
        
        /// @brief App state handle.
        struct
        {
            bool& is_logged_in;
            lms::User& logged_in_user;
            App::State::UserType& user_type;
        } app_state;

        /// @brief Table of user credentials.
        const services::Users& users_service;

        /// @section Private methods
        bool is_valid_username();
        bool is_valid_password() const;
        void populate_app_state_with_user_details();

    public:
        LoginPage(App::State& app_state, const services::Users& users_service, bool build = true);
        ~LoginPage() = default;

        void build();
        // void fetch_users_table();
};

inline LoginPage::LoginPage(App::State& app_state, const services::Users& users_service, const bool build)
    : app_state{
        .is_logged_in   = app_state.is_logged_in,
        .logged_in_user = app_state.logged_in_user,
        .user_type      = app_state.user_type
    },
    users_service{users_service}
{
    if (build)
        this->build();
}
