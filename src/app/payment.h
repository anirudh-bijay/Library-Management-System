#pragma once

#include "../objects/account.h"
#include "app.h"

class PaymentPage
{
    private:     
        /// @brief App state handle.
        struct
        {
            App::State::UserType& user_type;
            lms::Account& user_account;
        } app_state;

    public:
        PaymentPage(App::State& app_state, bool build = true);
        ~PaymentPage() = default;

        void build();
};

inline PaymentPage::PaymentPage(App::State& app_state, const bool build)
    : app_state{
        .user_type      = app_state.user_type,
        .user_account   = app_state.user_account
    }
{
    if (build)
        this->build();
}
