#include "../services/users.h"
#include "../services/accounts.h"
#include "../services/books.h"
#include "app.h"
#include "borrow_book.h"
#include "change_password.h"
#include "edit_books.h"
#include "home.h"
#include "login.h"
#include "manage_users.h"
#include "payment.h"
#include "return_book.h"
#include "search_books.h"
#include "welcome.h"

App::App(const bool build)
{
    if (build)
        this->build();
}

void App::build()
{
    while (true) {
        // This is the entry point to the program.

        WelcomePage();                          // Start the app with a welcome page.

        services::Users users_service;          /** @brief Service handling credentials table. */
        
        try {
            users_service.load();
        } catch (const std::runtime_error& ex) {
            std::cerr << ex.what() << std::endl;
            throw;
        } catch (const std::bad_alloc& ex) {
            std::cerr << ex.what() << std::endl;
            throw;
        }
        
        if (!state.is_logged_in) {
            LoginPage(state, users_service);
        }

        // We reach this point after the login form presented by the login page
        // is either submitted successfully or rejected.

        if (!state.is_logged_in)
            std::terminate();                   // Abort program on login failure.

        services::Accounts accounts_service;
        services::Books books_service;

        // Throw on failure to load tables. Abrupt program termination is
        // preferable to data corruption – if changes are made to a partially
        // loaded table and the result is dumped back into the file, the unloaded
        // rows would be overwritten!
        try {
            accounts_service.load();
            books_service.load();
        } catch (const std::runtime_error& ex) {
            std::cerr << ex.what() << std::endl;
            throw;
        } catch (const std::bad_alloc& ex) {
            std::cerr << ex.what() << std::endl;
            throw;
        }

        // We build the remaining pages depending on the user role.

        switch (state.user_type) {
            case App::State::UserType::LIBRARIAN:
                build_for_librarian(users_service, books_service, accounts_service);
                break;
            
            default:
                build_for_regular_user(users_service, books_service, accounts_service);
        }

        state.is_logged_in = false;
    }
}

void App::build_for_librarian(
    services::Users& users_service,
    services::Books& books_service,
    services::Accounts& accounts_service
)
{
    while (true) {
        typedef HomePage <State::UserType::LIBRARIAN>::Actions Actions;

        Actions next_action;
        HomePage <State::UserType::LIBRARIAN>{state, next_action};  // Build page

        if (next_action == Actions::LOGOUT) {
            break;
        }

        switch (next_action) {
            case Actions::EDIT_BOOKS: {
                bool record_created = false;

                EditBooksPage(books_service, record_created);
                if (record_created) {
                    books_service.dump();
                    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully committed changes\n" << ANSI_COLOR_RESET;
                    std::cout << "Press enter to continue...";
                    std::cin.ignore(std::numeric_limits <int>::max(), '\n');
                }
            }
            break;
            
            case Actions::MANAGE_USERS: {
                bool record_created = false;

                ManageUsersPage(users_service, accounts_service, record_created);
                if (record_created) {
                    users_service.dump();
                    accounts_service.dump();
                    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully committed changes\n" << ANSI_COLOR_RESET;
                    std::cout << "Press enter to continue...";
                    std::cin.ignore(std::numeric_limits <int>::max(), '\n');
                }
            }
            break;

            case Actions::CHANGE_PASSWORD: {
                ChangePasswordPage{state};
            }
            break;
        }
    }

    // Change password at logout
    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        if (std::get <0>(users_service.get(i)) == state.logged_in_user.getUserId()) {
            users_service.set <3>(i, state.logged_in_user.getPassword());
            users_service.dump();
            break;
        }
    }
}

void App::build_for_regular_user(
    services::Users& users_service,
    services::Books& books_service,
    services::Accounts& accounts_service
)
{
    // Load user account
    for (services::Accounts::size_type i = 0; i < accounts_service.size(); ++i) {
        if (accounts_service.get <0>(i) == state.logged_in_user.getUserId()) {
            state.user_account = accounts_service.get <1>(i);
            break;
        }
    }

    while (true) {
        typedef HomePage <State::UserType::STUDENT>::Actions Actions;

        Actions next_action;
        HomePage <State::UserType::STUDENT>{state, next_action};  // Build page

        if (next_action == Actions::LOGOUT) {
            break;
        }

        switch (next_action) {
            case Actions::SEARCH_BOOKS:
                SearchBooksPage{books_service};
                break;

            case Actions::BORROW: {
                BorrowBookPage(state, books_service);
                books_service.dump();
                for (services::Accounts::size_type i = 0; i < accounts_service.size(); ++i) {
                    if (accounts_service.get <0>(i) == state.logged_in_user.getUserId()) {
                        accounts_service.set <1>(i, state.user_account);
                        accounts_service.dump();
                        break;
                    }
                }
                std::cout << "Press enter to continue...";
                std::cin.ignore(std::numeric_limits <int>::max(), '\n');
            }
            break;

            case Actions::RETURN: {
                ReturnBookPage(state, books_service);
                books_service.dump();
                for (services::Accounts::size_type i = 0; i < accounts_service.size(); ++i) {
                    if (accounts_service.get <0>(i) == state.logged_in_user.getUserId()) {
                        accounts_service.set <1>(i, state.user_account);
                        accounts_service.dump();
                        break;
                    }
                }
                std::cout << "Press enter to continue...";
                std::cin.ignore(std::numeric_limits <int>::max(), '\n');
            }
            break;

            case Actions::PAYMENT: {
                if (state.user_type == App::State::UserType::FACULTY || state.user_account.returnedFine == 0)
                    PaymentPage{state};
                else {
                    PaymentPage{state};
                    if (state.user_account.returnedFine == 0) { // Fines cleared
                        for (services::Accounts::size_type i = 0; i < accounts_service.size(); ++i) {
                            if (accounts_service.get <0>(i) == state.logged_in_user.getUserId()) {
                                accounts_service.set <1>(i, state.user_account);
                                accounts_service.dump();
                                break;
                            }
                        }

                        std::cout << ANSI_COLOR_GREEN << "\nDues cleared!\n" << ANSI_COLOR_RESET;
                    }
                }
                std::cout << "Press enter to continue...";
                std::cin.ignore(std::numeric_limits <int>::max(), '\n');
            }
            break;

            case Actions::CHANGE_PASSWORD: {
                ChangePasswordPage{state};
            }
            break;
        }
    }

    // Change password at logout
    for (services::Users::size_type i = 0; i < users_service.size(); ++i) {
        if (std::get <0>(users_service.get(i)) == state.logged_in_user.getUserId()) {
            users_service.set <3>(i, state.logged_in_user.getPassword());
            users_service.dump();
            break;
        }
    }
}
