#pragma once

#include "app.h"

#include <iostream>

class LogoutPage
{ 
    private:        
        /// @brief App state handle.
        struct
        {
            bool& is_logged_in;
        } app_state;

    public:
        LogoutPage(App::State& app_state, bool build = true);
        ~LogoutPage() = default;

        void build();
};

inline LogoutPage::LogoutPage(App::State& app_state, const bool build)
    : app_state{
        .is_logged_in   = app_state.is_logged_in,
    }
{
    if (build)
        this->build();
}

inline void LogoutPage::build()
{
    app_state.is_logged_in = false;
    
    std::cout << "\nSuccessfully logged out!\n";
}
