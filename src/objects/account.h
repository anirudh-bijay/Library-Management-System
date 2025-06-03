#pragma once

#include "book.h"

#include <ctime>
#include <string>
#include <utility>
#include <vector>

namespace lms
{
    class Account;
} // namespace lms

namespace services
{
    class Accounts;
} // namespace services

class BorrowBookPage;
class ReturnBookPage;

class lms::Account
{
    friend class services::Accounts;
    friend class ::BorrowBookPage;
    friend class ::ReturnBookPage;
    
    public:
        using borrowed_book_detail_type = std::pair <lms::Book::bookId_type, std::time_t>;
        using returned_book_detail_type = borrowed_book_detail_type;

        using borrowed_book_list_type   = std::vector <borrowed_book_detail_type>;
        using returned_book_list_type   = std::vector <returned_book_detail_type>;
        
    private:
        /// @brief List of books borrowed but not returned along with their issue
        ///        dates.
        borrowed_book_list_type borrowedBooks;

        /// @brief List of books returned after borrowing along with their issue
        ///        dates.
        returned_book_list_type returnedBooks;

        borrowed_book_list_type& getBorrowedBookList() noexcept
        {
            return borrowedBooks;
        }
        
        returned_book_list_type& getReturnedBookList() noexcept
        {
            return returnedBooks;
        }

    public:
        unsigned returnedFine = 0;

        const borrowed_book_list_type& getBorrowedBookList() const noexcept
        {
            return borrowedBooks;
        }

        const returned_book_list_type& getReturnedBookList() const noexcept
        {
            return returnedBooks;
        }

    public:
        Account() = default;
        ~Account() = default;

        Account(unsigned returnedFine, borrowed_book_list_type&& borrowedBookList, returned_book_list_type&& returnedBookList)
        {
            returnedFine = returnedFine;
            borrowedBooks = std::move(borrowedBookList);
            returnedBooks = std::move(returnedBookList);
        }

        borrowed_book_list_type::size_type getBorrowedBookCount() const noexcept
        {
            return borrowedBooks.size();
        }

        returned_book_list_type::size_type getReturnedBookCount() const noexcept
        {
            return returnedBooks.size();
        }
};