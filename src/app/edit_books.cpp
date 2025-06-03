#include "../objects/book.h"
#include "../services/books.h"
#include "../utils/terminal_colours.h"
#include "edit_books.h"

#include <limits>

void EditBooksPage::build()
{
    while (true) {
        std::cout << '\n' <<
            "----------------------------------------\n"
            "                Actions                 \n"
            "----------------------------------------\n"
            "                                        \n"
            " 1. Add a record                        \n"
            " 2. Remove a record                     \n"
            " 3. Edit a record                       \n"
            " 4. Change book availability status     \n"
            " 5. Go back                             \n"
            "                                        \n"
            " Select a choice (1-5): ";

        unsigned choice;
        std::cin >> choice;
        while (!std::cin) {
            std::cin.clear();
            choice = 0;            
        }
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                buildAddRecordForm();
                break;

            case 2:
                buildRemoveRecordForm();
                break;

            case 3:
                buildEditRecordForm();
                break;

            case 4:
                buildChangeStatusForm();
                break;

            case 5:
                return;

            default:
                std::cout <<
                    ANSI_COLOR_MAGENTA << "Invalid choice\n" << ANSI_COLOR_RESET;
        }
    }
}

void EditBooksPage::buildAddRecordForm()
{
    lms::Book book;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "              Add a record              \n"
        "----------------------------------------\n"
        "                                        \n"
        " Book ID: ";
    
    std::cin >> book.bookId;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Book ID: ";
        std::cin >> book.bookId;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    
    for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
        if (std::get <0>(books_service.get(i)).bookId == book.bookId) {
            std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID is already assigned.\n" << ANSI_COLOR_RESET;
            return;
        }
    }

    std::cout << " ISBN: ";
    std::cin >> book.isbn;
    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    for (auto& c : book.isbn)
        if (c == '\\')
            c = '/';

    std::cout << " Title: ";
    std::getline(std::cin, book.title);
    
    for (auto& c : book.title)
        if (c == '\\')
            c = '/';

    std::cout << " Author: ";
    std::getline(std::cin, book.author);

    for (auto& c : book.author)
        if (c == '\\')
            c = '/';

    std::cout << " Publisher: ";
    std::getline(std::cin, book.publisher);

    for (auto& c : book.publisher)
        if (c == '\\')
            c = '/';

    std::cout << " Year: ";
    std::cin >> book.year;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Year: ";
        std::cin >> book.year;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
    
    std::cout << " Status (1: Reserved | 2: Borrowed | default: Available): ";
    unsigned status;
    std::cin >> status;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Status (1: Reserved | 2: Borrowed | default: Available): ";
        std::cin >> status;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    switch (status) {
        case 1: 
            book.status = lms::Book::Status::RESERVED;
            break;

        case 2:
            book.status = lms::Book::Status::BORROWED;
            break;

        default:
            book.status = lms::Book::Status::AVAILABLE;
    }

    books_service.append(services::Books::row_type{std::move(book)});
    record_created = true;
    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to add record\n" << ANSI_COLOR_RESET;
}

void EditBooksPage::buildRemoveRecordForm()
{
    lms::Book::bookId_type bookId;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "            Remove a record             \n"
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
            books_service.erase(i);
            record_created = true;
            std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to delete record\n" << ANSI_COLOR_RESET;
            return;
        }
    }

    std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID does not exist.\n" << ANSI_COLOR_RESET;
    return;
}

void EditBooksPage::buildEditRecordForm()
{
    lms::Book book;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "               Edit a record            \n"
        "----------------------------------------\n"
        "                                        \n"
        " Book ID: ";
    
    std::cin >> book.bookId;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Book ID: ";
        std::cin >> book.bookId;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    bool exists = false;
    services::Books::size_type i;
    
    for (i = 0; i < books_service.size(); ++i) {
        if (std::get <0>(books_service.get(i)).bookId == book.bookId) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID does not exist.\n" << ANSI_COLOR_RESET;
        return;
    }

    std::cout << " ISBN " << "(current: " << std::get <0>(books_service.get(i)).isbn << ")";
    std::cin >> book.isbn;
    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    for (auto& c : book.isbn)
        if (c == '\\')
            c = '/';

    std::cout << " Title " << "(current: " << std::get <0>(books_service.get(i)).title << ")";
    std::getline(std::cin, book.title);
    
    for (auto& c : book.title)
        if (c == '\\')
            c = '/';

    std::cout << " Author " << "(current: " << std::get <0>(books_service.get(i)).author << ")";
    std::getline(std::cin, book.author);

    for (auto& c : book.author)
        if (c == '\\')
            c = '/';

    std::cout << " Publisher " << "(current: " << std::get <0>(books_service.get(i)).publisher << ")";
    std::getline(std::cin, book.publisher);

    for (auto& c : book.publisher)
        if (c == '\\')
            c = '/';

    std::cout << " Year " << "(current: " << std::get <0>(books_service.get(i)).year << ")";
    std::cin >> book.year;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Year " << "(current: " << std::get <0>(books_service.get(i)).year << ")";
        std::cin >> book.year;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    book.status = std::get <0>(books_service.get(i)).status;

    books_service.set(i, services::Books::row_type{std::move(book)});
    record_created = true;
    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to edit record\n" << ANSI_COLOR_RESET;
}

void EditBooksPage::buildChangeStatusForm()
{
    lms::Book::bookId_type bookId;

    std::cout << '\n' <<
        "----------------------------------------\n"
        "       Change availability status       \n"
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

    bool exists = false;
    services::Books::size_type i;
    
    for (i = 0; i < books_service.size(); ++i) {
        if (std::get <0>(books_service.get(i)).bookId == bookId) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        std::cout << ANSI_COLOR_CYAN << "\nSorry, this ID does not exist.\n" << ANSI_COLOR_RESET;
        return;
    }

    std::cout << " Status (1: Reserved | 2: Borrowed | default: Available): ";
    unsigned status;
    std::cin >> status;

    while (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
        std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
        std::cout << " Status (1: Reserved | 2: Borrowed | default: Available): ";
        std::cin >> status;
    }

    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

    auto book = std::get <0>(books_service.get(i));

    switch (status) {
        case 1: 
            book.status = lms::Book::Status::RESERVED;
            break;

        case 2:
            book.status = lms::Book::Status::BORROWED;
            break;

        default:
            book.status = lms::Book::Status::AVAILABLE;
    }

    books_service.set <0>(i, std::move(book));
    record_created = true;
    std::cout << ANSI_COLOR_GREEN << "\nSuccessfully registered request to change status\n" << ANSI_COLOR_RESET;
}
