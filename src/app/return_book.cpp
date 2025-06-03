#include "../objects/book.h"
#include "../services/books.h"
#include "../utils/terminal_colours.h"
#include "return_book.h"

#include <algorithm>
#include <ctime>
#include <limits>

void ReturnBookPage::build()
{
    lms::Book::bookId_type bookId;
    
    std::cout << '\n' <<
        "----------------------------------------\n"
        "             Return a book              \n"
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

    auto it = std::find_if(
        app_state.user_account.getBorrowedBookList().begin(),
        app_state.user_account.getBorrowedBookList().end(),
        [bookId](const auto& elem) -> bool {
            return elem.first == bookId;
        }
    );

    if (it == app_state.user_account.getBorrowedBookList().end()) {
        std::cout << ANSI_COLOR_RED << "This book has not been borrowed.\n" << ANSI_COLOR_RESET;
        return;
    }

    app_state.user_account.getReturnedBookList().push_back(*it);
    if (app_state.user_type == App::State::UserType::STUDENT) {
        const auto& s_user =  static_cast <const lms::Student&>(app_state.logged_in_user);
        unsigned returnedAfter = std::difftime(std::time(nullptr), it->second) / 86400;
        if (returnedAfter > s_user.maxBorrowPeriod) {
            app_state.user_account.returnedFine += s_user.fineRate * (returnedAfter - s_user.maxBorrowPeriod);
            std::cout << "\nOverdue by " << (returnedAfter - s_user.maxBorrowPeriod) << " days!\n";
        }
    } else {
        const auto& f_user =  static_cast <const lms::Faculty&>(app_state.logged_in_user);
        auto returnedAfter = std::difftime(std::time(nullptr), it->second) / 86400;
        if (returnedAfter > f_user.maxBorrowPeriod) {
            std::cout << "\nOverdue by " << unsigned(returnedAfter - f_user.maxBorrowPeriod) << " days!\n";
        }
    }
    app_state.user_account.getBorrowedBookList().erase(it);
    
    for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
        if (std::get <0>(books_service.get(i)).bookId == bookId) {
            auto book = std::get <0>(books_service.get(i));
            if (book.status != lms::Book::Status::RESERVED) {
                book.status = lms::Book::Status::AVAILABLE;
                books_service.set <0>(i, book);
            }
            break;
        }
    }

    std::cout << ANSI_COLOR_GREEN << "\nRegistered request\n" << ANSI_COLOR_RESET;
    return;
}
