#include "../objects/book.h"
#include "../services/books.h"
#include "../utils/terminal_colours.h"
#include "search_books.h"

#include <limits>

void SearchBooksPage::build()
{
    while (true) {
        std::cout << '\n' <<
            "----------------------------------------\n"
            "               Search by:               \n"
            "----------------------------------------\n"
            "                                        \n"
            " 1. ISBN                                \n"
            " 2. Title                               \n"
            " 3. Author                              \n"
            " 4. Publisher                           \n"
            " 5. Year                                \n"
            " 6. Go back                             \n"
            "                                        \n"
            " Select a choice (1-6): ";

        unsigned choice;
        std::cin >> choice;
        while (!std::cin) {
            std::cin.clear();
            choice = 0;
        }
        std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                lms::Book::isbn_type isbn;
                std::cout << "\n ISBN: ";
                std::cin >> isbn;
                std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
                for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
                    if (std::get <0>(books_service.get(i)).isbn == isbn) {
                        pretty_print(std::get <0>(books_service.get(i)));
                    }
                }
            }
            break;

            case 2: {
                lms::Book::title_type title;
                std::cout << "\n Title: ";
                std::getline(std::cin, title);
                for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
                    if (std::get <0>(books_service.get(i)).title == title) {
                        pretty_print(std::get <0>(books_service.get(i)));
                    }
                }
            }
            break;

            case 3: {
                lms::Book::author_type author;
                std::cout << "\n Author: ";
                std::getline(std::cin, author);
                for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
                    if (std::get <0>(books_service.get(i)).author == author) {
                        pretty_print(std::get <0>(books_service.get(i)));
                    }
                }
            }
            break;

            case 4: {
                lms::Book::publisher_type publisher;
                std::cout << "\n Publisher: ";
                std::getline(std::cin, publisher);
                for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
                    if (std::get <0>(books_service.get(i)).publisher == publisher) {
                        pretty_print(std::get <0>(books_service.get(i)));
                    }
                }
            }
            break;

            case 5: {
                lms::Book::year_type year;
                std::cout << "\n Year: ";
                std::cin >> year;

                while (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
                    std::cout << ANSI_COLOR_RED << "Bad input! Please try again.\n" << ANSI_COLOR_RESET;
                    std::cout << " Year: ";
                    std::cin >> year;
                }

                std::cin.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
                for (services::Books::size_type i = 0; i < books_service.size(); ++i) {
                    if (std::get <0>(books_service.get(i)).year == year) {
                        pretty_print(std::get <0>(books_service.get(i)));
                    }
                }
            }
            break;

            case 6:
                return;

            default:
                std::cout <<
                    ANSI_COLOR_MAGENTA << "Invalid choice\n" << ANSI_COLOR_RESET;
        }
    }
}
