#pragma once

#include "../objects/book.h"
#include "../utils/ssv_handler.h"
#include "base.h"

#include <utility>

namespace services
{
    class Books;
} // namespace services

class services::Books : public BaseService <services::Books>
{
    friend BaseService;
    
    public:
        /// @brief File from which to load the table.
        static constexpr const char* filename = "./data/books.ssv";

    private:
        ssv::Table handle{filename};    // File handle.
        ssv::Table::table_type table;   // In-memory table.

    public:
        /// @brief Type of rows accepted and returned by methods. Same as
        ///        @c `decltype(Books().get(0))`.
        using row_type = std::tuple <lms::Book>;

        Books() = default;
        ~Books() = default;

        /// @brief Get the `n`th row of the table.
        /// @param n Row index (zero-indexed).
        /// @return The `n`th row.
        row_type get(difference_type n) const;

        /// @brief Update the column at the index specified by the template
        ///        parameter @c `Col` of the row at index @c `Row` with the
        ///        value @c `val`.
        /// @tparam Col Column index (zero-indexed). Must be known at
        ///         compile-time.
        /// @param Row Row index (zero-indexed).
        /// @param val Value to update with.
        template <difference_type Col>
        void set(
            difference_type Row,
            const std::tuple_element_t <Col, row_type>& val
        );

        public:
            using BaseService::set;
};

/// @brief Update the column at the index specified by the template
///        parameter @c `Col` of the row at index @c `Row` with the
///        value @c `val`.
/// @tparam Col Column index (zero-indexed). Must be known at
///         compile-time.
/// @param Row Row index (zero-indexed).
/// @param val Value to update with.
template <services::Books::difference_type Col>
inline void services::Books::set(
    const difference_type Row,
    const std::tuple_element_t <Col, row_type>& val
)
{
    if (table.at(Row).size() <= Col) {
        table[Row].resize(Col + 1);
    }
    
    table.at(Row) = ssv::Table::table_type::value_type{
        std::to_string(val.bookId),
        val.isbn,
        val.title,
        val.author,
        val.publisher,
        std::to_string(val.year),
        std::to_string(val.status)
    };
}
