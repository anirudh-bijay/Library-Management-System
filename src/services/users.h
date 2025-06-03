#pragma once

#include "../objects/users.h"
#include "../utils/ssv_handler.h"
#include "base.h"

#include <string>
#include <utility>

namespace services
{
    class Users;
} // namespace services

class services::Users : public BaseService <services::Users>
{
    friend BaseService;

    public:
        /// @brief File from which to load the table.
        static constexpr const char* filename = "./data/users.ssv";

    private:
        ssv::Table handle{filename};    // File handle.
        ssv::Table::table_type table;   // In-memory table.
        
    public:
        enum UserType : std::uint_least8_t {
            LIBRARIAN, STUDENT, FACULTY
        };

        /// @brief Type of rows accepted and returned by methods. Same as
        ///        @c `decltype(Users().get(0))`.
        /// 
        ///        Index 0: User id
        /// 
        ///        Index 1: Username
        /// 
        ///        Index 2: User role
        /// 
        ///        Index 3: Password
        using row_type = std::tuple <
            lms::User::userId_type,
            std::string,
            UserType,
            lms::User::password_type
        >;

        Users() = default;
        ~Users() = default;

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
template <services::Users::difference_type Col>
inline void services::Users::set(
    const difference_type Row,
    const std::tuple_element_t <Col, row_type>& val
)
{
    if (table.at(Row).size() <= Col) {
        table[Row].resize(Col + 1);
    }
    
    if constexpr (Col == 0) {
        table.at(Row).at(0) = std::to_string(val);
    } else if constexpr (Col == 1) {
        table.at(Row).at(1) = val;
    } else if constexpr (Col == 2) {
        table.at(Row).at(2) = std::to_string(val);
    } else {
        table.at(Row).at(3) = val;
        table.at(Row).resize(4);
    }
}
