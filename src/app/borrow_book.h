#pragma once

#include "../services/books.h"
#include "../objects/users.h"
#include "../objects/account.h"
#include "app.h"

class BorrowBookPage
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

        /// @section Private methods
        bool eligible_to_borrow() const;

    public:
        BorrowBookPage(App::State& app_state, services::Books& books_service, bool build = true);
        ~BorrowBookPage() = default;

        void build();
};

inline BorrowBookPage::BorrowBookPage(App::State& app_state, services::Books& books_service, const bool build)
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
