#pragma once

#include "../objects/account.h"
#include "../objects/users.h"
#include "../utils/ssv_handler.h"
#include "base.h"

namespace services
{
    class Accounts;
} // namespace services

class services::Accounts : public BaseService <services::Accounts>
{
    friend BaseService;
    
    public:
        /// @brief File from which to load the table.
        static constexpr const char* filename = "./data/accounts.ssv";

    private:
        ssv::Table handle{filename};    // File handle.
        ssv::Table::table_type table;   // In-memory table.

    public:
        /// @brief Type of rows accepted and returned by methods. Same as
        ///        @c `decltype(Accounts().get(0))`.
        using row_type = std::tuple <
            lms::User::userId_type,
            lms::Account
        >;

        Accounts() = default;
        ~Accounts() = default;

        /// @brief Get the `n`th row of the table.
        /// @param n Row index (zero-indexed).
        /// @return The `n`th row.
        row_type get(difference_type n) const;

        /// @brief Get the column at the index specified by the template
        ///        parameter @c `Col` of the row at index @c `Row` with the
        ///        value @c `val`.
        /// @tparam Col Column index (zero-indexed). Must be known at
        ///         compile-time.
        /// @param Row Row index (zero-indexed).
        template <services::Accounts::difference_type Col>
        inline std::tuple_element_t <Col, row_type> get(const difference_type Row) const;

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
template <services::Accounts::difference_type Col>
inline void services::Accounts::set(
    const difference_type Row,
    const std::tuple_element_t <Col, row_type>& val
)
{    
    if constexpr (Col == 0u) {
        if (table.at(Row).size() <= Col) {
            table[Row].resize(Col + 1);
        }
        table[Row][0] = std::to_string(val);
    } else {
        table.at(Row).resize(2 + 2 * val.getBorrowedBookCount() + 1 + 2 * val.getReturnedBookCount());
        table[Row][1] = std::to_string(val.returnedFine);

        for (size_type i = 0; i < val.getBorrowedBookCount(); ++i) {
            table[Row][2 * (i + 1)] = std::to_string(val.getBorrowedBookList()[i].first);
            table[Row][2 * (i + 1) + 1] = std::to_string(val.getBorrowedBookList()[i].second);
        }

        const auto separator_pos = 2 * (val.getBorrowedBookCount() + 1);    // Separator index

        table[Row][separator_pos] = "!";                                    // Separator

        for (size_type i = 0; i < val.getReturnedBookCount(); ++i) {
            table[Row][2 * i + 1 + separator_pos] = std::to_string(val.getReturnedBookList()[i].first);
            table[Row][2 * (i + 1) + separator_pos] = std::to_string(val.getReturnedBookList()[i].second);
        }
    }
}

/// @brief Get the column at the index specified by the template
///        parameter @c `Col` of the row at index @c `Row` with the
///        value @c `val`.
/// @tparam Col Column index (zero-indexed). Must be known at
///         compile-time.
/// @param Row Row index (zero-indexed).
template <services::Accounts::difference_type Col>
inline std::tuple_element_t <Col, services::Accounts::row_type>
services::Accounts::get(const difference_type Row) const
{
    const auto& row = table.at(Row);

    if constexpr (Col == 0u) {
        return std::strtoull(row.at(0).c_str(), nullptr, 10);
    } else {
        lms::Account::borrowed_book_list_type blist, rlist;

        if (row.cbegin() + 2 == row.cend())
            throw std::runtime_error(
                std::string("Row ").append(std::to_string(Row))
                    .append(" of accounts.ssv is corrupted")
            );

        for (auto it = row.cbegin() + 2; *it != "!"; it += 2) {
            blist.emplace_back(
                std::strtoull(it->c_str(), nullptr, 10),
                std::strtoull((it + 1)->c_str(), nullptr, 10)
            );
        }

        for (auto it = row.cbegin() + 2 * blist.size() + 3; it != row.cend(); it += 2) {
            rlist.emplace_back(
                std::strtoull(it->c_str(), nullptr, 10),
                std::strtoull((it + 1)->c_str(), nullptr, 10)
            );
        }

        return lms::Account(std::stoul(row.at(1)), std::move(blist), std::move(rlist));
    }
}
