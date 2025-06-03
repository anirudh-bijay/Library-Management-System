#include "../utils/terminal_colours.h"
#include "app.h"
#include "login.h"

#include <limits>

/*
bool LoginPage::is_valid_username() const
{
    // Sanitise input. As the sanitiser is rudimentary, we impose the
    // restriction that usernames consist only of alphanumeric characters.
    if (sql_sanitiser::is_alphanumeric(state.username) == false)
        return false;

    try {
        dbConnection conn("../database/lms.sqlite");
        conn.connect(dbConnection::ConnectionMode::READ_ONLY);

        conn.execute(
            std::string("SELECT 1 FROM users WHERE username = '")
            .append(state.username)
            .append("' LIMIT 1;")
        );

        switch (conn.fetchone()) {
            case SQLITE_ROW:
                return true;

            case SQLITE_DONE:
                return false;

            default:
                throw std::runtime_error("Database error");
        }

    } catch (const std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
        throw;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
        
    }
}

bool LoginPage::is_valid_password() const
{
    try {
        dbConnection conn("../database/lms.sqlite");
        conn.connect(dbConnection::ConnectionMode::READ_ONLY);

        conn.execute(
            std::string("SELECT password FROM users WHERE username = '")
            .append(state.username)
            .append("' LIMIT 1;")
        );

        if (conn.fetchone() == SQLITE_ROW) {
            const std::string retrieved_password{conn.getColumn(1)};
            return state.password == retrieved_password;
        } else
            throw std::runtime_error("Database error");

    } catch (const std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
        throw;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
        
    }
}

void LoginPage::populate_app_state_with_user_details()
{
    try {
        dbConnection conn("../database/lms.sqlite");
        conn.connect(dbConnection::ConnectionMode::READ_ONLY);

        conn.execute(
            std::string("SELECT id, role FROM users WHERE username = '")
            .append(state.username)
            .append("' LIMIT 1;")
        );

        if (conn.fetchone() == SQLITE_ROW) {
            app_state.logged_in_user = lms::User{std::size_t{conn.getColumn(0)}};
            app_state.user_type = App::State::UserType{int{conn.getColumn(1)}};
        } else
            throw std::runtime_error("Database error");

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout << ANSI_COLOR_RED << "Oops! An unexpected error occurred.\n" << ANSI_COLOR_RESET;

        throw;
    }
}
*/

bool LoginPage::is_valid_username()
{
    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        const auto row = users_service.get(i);
        if (std::get <1>(row) == state.username) {
            state.user_row = std::move(row);
            return true;
        }
    }

    return false;
}

bool LoginPage::is_valid_password() const
{
    return std::get <3>(state.user_row) == state.password;
}

void LoginPage::populate_app_state_with_user_details()
{
    app_state.logged_in_user = lms::User(
        std::get <0>(state.user_row),                       // User id
        state.password                                      // Password
    );
    app_state.user_type = std::get <2>(state.user_row);     // User role
}

void LoginPage::build()
{
    app_state.is_logged_in = false; // Just to be sure.
    
    /// @section Username input
    while (true) {
        // Take username as input. Loop until a valid username is entered.

        std::cout << "Username: ";
        // std::cin >> state.username;
        std::getline(std::cin, state.username);
        std::cout << "\n\n";

        if (is_valid_username())
            break;

        std::cout <<
            ANSI_COLOR_RED << "Invalid username.\n\n" << ANSI_COLOR_RESET;
    }
    
    /// @section Password input
    unsigned int attempts;          // Number of failed password attempts.
    for (attempts = max_password_attempts; attempts > 0; --attempts) {
        // Take password as input. Stop after five incorrect attempts.

        std::cout << "Password for user " << state.username << ": ";
        std::getline(std::cin, state.password);
        std::cout << "\n\n";

        if (is_valid_password())
            break;

        std::cout <<
            ANSI_COLOR_RED << "Incorrect password; " << ANSI_COLOR_RESET;

        if (attempts == 1)
            std::cout << "terminating program.\n\n";
        else if (attempts == 2)
            std::cout << "1 attempt left.\n\n";
        else
            std::cout << (attempts - 1) << " attempts left.\n\n";
    }

    /// Abort the program after @c `max_password_attempts` invalid attempts.
    /// This helps mitigate small-scale brute-force attacks.
    /// Control is returned to the caller; abortion shall be performed by the
    /// caller on inspection of the login status.
    if (attempts == 0)
        return;

    populate_app_state_with_user_details();
    app_state.is_logged_in = true;
}
