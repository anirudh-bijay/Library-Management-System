#pragma once

#include "../objects/account.h"
#include "../objects/users.h"
#include "../services/accounts.h"
#include "../services/books.h"
#include "../services/users.h"

class App
{
    public:
        /// @brief App state. POD `struct`.
        struct State
        {
            using UserType = services::Users::UserType;

            /// @brief `true` if a user is currently logged in; `false` otherwise.
            bool is_logged_in = false;

            /// @brief Details of the user currently logged in. Meaningful only
            ///        if @c `App::State::is_logged_in` is `true`.
            lms::User logged_in_user;

            /// @brief Role of the user currently logged in. Meaningful only
            ///        if @c `App::State::is_logged_in` is `true`.
            /// 
            ///        This is used to decide the appropriate user class
            ///        (derived from `lms::User`) to cast
            ///        @c `App::State::logged_in_user` to for runtime
            ///        polymorphism/dynamic dispatch.
            UserType user_type;

            /// @brief Account of the user currently logged in. Meaningful only
            ///        if @c `App::State::is_logged_in` is `true` and
            ///        @c `App::State::user_type` is `false`.
            lms::Account user_account;
        };

    private:
        State state;                    /** @brief App state. */
        
        void build_for_librarian(
            services::Users& users_service,
            services::Books& books_service,
            services::Accounts& accounts_service
        );

        void build_for_regular_user(
            services::Users& users_service,
            services::Books& books_service,
            services::Accounts& accounts_service
        );

    public:
        App(bool build = true);
        ~App() = default;

        void build();
};
