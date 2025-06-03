#pragma once

#include "../utils/ssv_handler.h"

namespace services
{
    template <class Derived>
    class BaseService;
} // namespace services

/// @brief Base class for services that handle data tables stored as ssv
///        files. Inherited using CRTP for static dispatch.
/// 
/// @tparam Derived Class being derived through CRTP.
/// 
/// @note `Derived` must have the nested type/member typedef `row_type`.
///       `row_type` should be a tuple or emulate one.
/// 
///       Further, `Derived` must have the templated member function
/// 
///       ```c++
///       template <difference_type Col>
///       Derived::set(difference_type Row, const std::tuple_element_t <Col, Derived::row_type>& val)
///       ```
/// 
///       that sets the contents of the cell at row `Row` and column `Col` to
///       the value `val`.
template <class Derived>
class services::BaseService
{
    public:
        /// @brief Type used for indices.
        using difference_type   = ssv::Table::table_type::difference_type;

        /// @brief Result type of @c `size()`.
        using size_type         = ssv::Table::table_type::size_type;

        /// @brief Load the table into memory from the file at
        ///        @c `filename`.
        void load()
        {
            static_cast <Derived*>(this)->handle.load();
            static_cast <Derived*>(this)->table = std::move(static_cast <Derived*>(this)->handle).get_table();
        }

        /// @brief Dump the in-memory table to the file at
        ///        @c `filename`. In other words, commit changes
        ///        to the file.
        void dump()
        {
            static_cast <Derived*>(this)->handle.set_table(std::move(static_cast <Derived*>(this)->table));
            static_cast <Derived*>(this)->handle.dump();
            static_cast <Derived*>(this)->table = std::move(static_cast <Derived*>(this)->handle).get_table();
        }

    private:
        /// @brief Helper function for @c `set()` (the overload that sets a
        ///        whole row, not individual columns) to set columns
        ///        individually by iterating over the
        ///        elements of the tuple represented by @c `row_type`.
        /// 
        /// @tparam Col Index of the element of the row to set (zero-indexed).
        /// 
        /// @param Row Index of the row to update (zero-indexed).
        /// 
        /// @param row Contents to update the row with.
        template <difference_type Col = 0, class D = Derived>
        void set_helper(const difference_type Row, const typename D::row_type& row)
        {
            if constexpr (Col == std::tuple_size_v <typename D::row_type>) {
                return;
            } else {
                static_cast <Derived*>(this)->template set <Col>(Row, std::get <Col>(row));
                set_helper <Col + 1, D>(Row, row);
            }
        }

    public:
        /// @brief Update the column at the index specified by the template
        ///        parameter @c `Col` of the row at index @c `Row` with the
        ///        value @c `val`.
        /// 
        /// @tparam Col Column index (zero-indexed). Must be known at
        ///         compile-time.
        /// 
        /// @param Row Row index (zero-indexed).
        /// 
        /// @param val Value to update with.
        template <class D = Derived>
        void set(const difference_type Row, const typename D::row_type& row)
        {
            set_helper(Row, row);
        }

        /// @brief Get the number of rows in the table.
        /// @return Count of rows in the table.
        size_type size() const noexcept
        {
            return static_cast <const Derived*>(this)->table.size();
        }

        /// @brief Remove the `n`th row from the table.
        /// @param n Row index (zero-indexed). 
        void erase(const difference_type n)
        {
            static_cast <Derived*>(this)->table.erase(static_cast <Derived*>(this)->table.cbegin() + n);
        }

        /// @brief Append a row to the table.
        /// @param row Row to append to the table.
        template <class D = Derived>
        void append(const typename D::row_type &row)
        {
            static_cast <Derived*>(this)->table.emplace_back();
            set(size() - 1, row);
        }
};
