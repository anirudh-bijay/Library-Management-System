#include "../objects/book.h"
#include "../utils/terminal_colours.h"
#include "payment.h"

#include <limits>

void PaymentPage::build()
{
    lms::Book::bookId_type bookId;
    
    std::cout << '\n' <<
        "----------------------------------------\n"
        "              Payment Page              \n"
        "----------------------------------------\n"
        "                                        \n";

    if (app_state.user_type == App::State::UserType::FACULTY || app_state.user_account.returnedFine == 0) {
        std::cout << " No dues to pay\n";
        return;
    } else {
        std::cout << " Dues to pay: Rs. " << app_state.user_account.returnedFine << '\n';
        std::cout << " Clear fines? (1: Yes, default: No)\n > ";
    }

    unsigned choice;
    std::cin >> choice;
    
    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Dues to pay: Rs. " << app_state.user_account.returnedFine << '\n';
        std::cout << " Clear fines? (1: Yes, default: No)\n > ";
        std::cin >> choice;
    }
    
    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    if (choice == 1)
        app_state.user_account.returnedFine = 0;

    return;
}
