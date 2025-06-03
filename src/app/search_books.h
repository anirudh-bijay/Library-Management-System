#pragma once

#include "../objects/book.h"
#include "../services/books.h"

#include <iostream>

class SearchBooksPage
{ 
    private:
        // This page is stateless.

        /// @brief Table of books.
        const services::Books& books_service;

        /// @section Private methods
        static void pretty_print(const lms::Book& book);

    public:
        SearchBooksPage(
            const services::Books& books_service,
            bool build = true
        );
        ~SearchBooksPage() = default;

        void build();
};

inline void SearchBooksPage::pretty_print(const lms::Book &book)
{
    std::cout << '\n';
    std::cout << " Book ID: "   << book.bookId << '\n';
    std::cout << " ISBN: "      << book.isbn << '\n';
    std::cout << " Title: "     << book.title << '\n';
    std::cout << " Author: "    << book.author << '\n';
    std::cout << " Publisher: " << book.publisher << '\n';
    std::cout << " Year: "      << book.year << '\n';
    std::cout << " Status: ";

    switch (book.status) {
        case lms::Book::Status::AVAILABLE:
            std::cout << "Available\n";
            break;
        
        case lms::Book::Status::BORROWED:
            std::cout << "Borrowed\n";
            break;

        case lms::Book::Status::RESERVED:
            std::cout << "Reserved\n";
    }

}

inline SearchBooksPage::SearchBooksPage(
    const services::Books &books_service,
    const bool build
) : books_service{books_service}
{
    if (build)
        this->build();
}
