#pragma once

#include "../objects/account.h"
#include "../objects/users.h"
#include "../services/users.h"
#include "app.h"

class ReturnBookPage
{
    private:     
        /// @brief App state handle.
        struct
        {
            lms::User& logged_in_user;
            App::State::UserType& user_type;
            lms::Account& user_account;
        } app_state;

        /// @brief Table of books.
        services::Books& books_service;

    public:
        ReturnBookPage(App::State& app_state, services::Books& books_service, bool build = true);
        ~ReturnBookPage() = default;

        void build();
};

inline ReturnBookPage::ReturnBookPage(App::State& app_state, services::Books& books_service, const bool build)
    : app_state{
        .logged_in_user = app_state.logged_in_user,
        .user_type      = app_state.user_type,
        .user_account   = app_state.user_account
    },
    books_service{books_service}
{
    if (build)
        this->build();
}
