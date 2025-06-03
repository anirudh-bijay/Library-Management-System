#include "../objects/book.h"
#include "../services/books.h"
#include "../utils/terminal_colours.h"
#include "borrow_book.h"

#include <algorithm>
#include <ctime>
#include <limits>

void BorrowBookPage::build()
{
    lms::Book::bookId_type bookId;
    
    std::cout << '\n' <<
        "----------------------------------------\n"
        "             Borrow a book              \n"
        "----------------------------------------\n"
        "                                        \n"
        " Book ID: ";
    
    std::cin >> bookId;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Book ID: ";
        std::cin >> bookId;
    }
    
    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    
    for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
        if (std::get <0>(books_service.get(i)).bookId == bookId) {
            auto book = std::get <0>(books_service.get(i));
            switch (book.status) {
                case lms::Book::Status::RESERVED:
                case lms::Book::Status::BORROWED:
                    std::cout << "\nSorry, this book is not available to borrow.\n";
                    return;
            }
            
            if (eligible_to_borrow()) {
                book.status = lms::Book::Status::BORROWED;
                books_service.set <0>(i, book);
                app_state.user_account.getBorrowedBookList().emplace_back(bookId, std::time(nullptr));
                std::cout << ANSI_COLOR_GREEN << "\nRegistered request\n" << ANSI_COLOR_RESET;
            } else {
                std::cout << "\nSorry, you are not eligible to borrow.\n";
            }
            return;
        }
    }

    std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID does not exist.\n" << ANSI_COLOR_RESET;
    return;
}

/// @brief Check if the logged in user is eligible to borrow books.
/// @return `true` if eligible, else `false`.
bool BorrowBookPage::eligible_to_borrow() const
{
    if (app_state.user_type == App::State::UserType::FACULTY) {
        const lms::Faculty& f_user = static_cast <const lms::Faculty&>(app_state.logged_in_user);

        if (app_state.user_account.getBorrowedBookCount() >= f_user.maxBorrowLimit)
            return false;

        if (std::find_if(
            app_state.user_account.getBorrowedBookList().cbegin(),
            app_state.user_account.getBorrowedBookList().cend(),
            [&f_user](const lms::Account::borrowed_book_detail_type& elem) -> bool {
                return std::difftime(std::time(nullptr), elem.second) > 60 * 86400;
            }
        ) != app_state.user_account.getBorrowedBookList().cend()) {
            return false;
        }

        return true;
    } else {
        const lms::Student& s_user = static_cast <const lms::Student&>(app_state.logged_in_user);

        if (app_state.user_account.getBorrowedBookCount() >= s_user.maxBorrowLimit)
            return false;

        if (std::find_if(
            app_state.user_account.getBorrowedBookList().cbegin(),
            app_state.user_account.getBorrowedBookList().cend(),
            [&s_user](const lms::Account::borrowed_book_detail_type& elem) -> bool {
                return std::difftime(std::time(nullptr), elem.second) > s_user.maxBorrowPeriod * 86400;
            }
        ) != app_state.user_account.getBorrowedBookList().cend()) {
            return false;
        }

        if (app_state.user_account.returnedFine > 0)
            return false;

        return true;
    }
}
