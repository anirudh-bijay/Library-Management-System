#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace lms
{
    class Book;
} // namespace lms

namespace services
{
    class Books;
} // namespace services

class EditBooksPage;
class SearchBooksPage;
class BorrowBookPage;
class ReturnBookPage;

struct lms::Book
{
    // friend struct std::hash <Book>;
    friend class services::Books;
    friend class ::EditBooksPage;
    friend class ::SearchBooksPage;
    friend class ::BorrowBookPage;
    friend class ::ReturnBookPage;

    public:
        enum Status : std::uint_least8_t {
            AVAILABLE, RESERVED, BORROWED
        };

    private:
        std::uint64_t bookId;
        std::string isbn, title, author, publisher;
        std::uint16_t year;
        enum Status status;

    public:
        using bookId_type       = decltype(bookId);
        using isbn_type         = decltype(isbn);
        using title_type        = decltype(title);
        using author_type       = decltype(author);
        using publisher_type    = decltype(publisher);
        using year_type         = decltype(year);
        using status_type       = decltype(status);

        Book() = default;

        Book(
            bookId_type bookId,
            const std::string& isbn,
            const std::string& title,
            const std::string& author,
            const std::string& publisher,
            std::uint16_t year,
            enum Status status
        ) : bookId{bookId}, isbn{isbn}, title{title}, author{author}, publisher{publisher},
            year{std::move(year)}, status{std::move(status)} {}

        Book(
            bookId_type bookId,
            std::string&& isbn,
            std::string&& title,
            std::string&& author,
            std::string&& publisher,
            std::uint16_t year,
            enum Status status
        ) noexcept : bookId{bookId}, isbn{std::move(isbn)}, title{std::move(title)},
            author{std::move(author)}, publisher{std::move(publisher)}, year{std::move(year)},
            status{std::move(status)} {}

        /// @brief Move constructor.
        Book(Book&&) noexcept = default;

    private:
        /// @brief Copy constructor (private).
        Book(const Book&) = default;

    public:
        /// @brief Compare two book entries by their respective id's.
        /// @param lhs Book record of type @c `lms::Book`.
        /// @param rhs Book record of type @c `lms::Book`.
        /// @return `true` if the id's match, else `false`.
        friend bool operator==(const lms::Book& lhs, const lms::Book& rhs) noexcept
        {
            return lhs.bookId == rhs.bookId;
        }
};

// /// @brief Specialisation for @c `lms::Book`. The hash is computed from the id
// ///        of the book.
// template <>
// struct std::hash <lms::Book>
// {
//     constexpr std::size_t operator()(const lms::Book& key) const noexcept
//     {
//         return std::hash <lms::Book::bookId_type>{}(key.bookId);
//     }
// };
