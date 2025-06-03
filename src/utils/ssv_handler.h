/**
 * @file ssv_handler.h
 * Handler to read from and write to .ssv files.
 * 
 * .ssv files are analogous to .csv files, but use a backslash ('\\') as the
 * delimiter instead of a comma (','). This permits the use of commas
 * within the data without requiring additional escaping.
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ssv
{
    class Table;
} // namespace ssv

class ssv::Table
{
    public:
        using table_type    = std::vector <std::vector <std::string> >;
        using size_type     = table_type::value_type::size_type;

    private:
        /** @brief Filename. */
        std::string filename;

        /** @brief Table loaded into memory. */
        table_type table;

        /** @brief Expected number of columns. */
        size_type reserve_columns = 0;

        void write_table_to_file(std::ofstream &f) const;

    public:
        Table(const std::string&);
        Table(std::string&&);
        ~Table() = default;

        Table(const Table&) = delete;
        Table& operator=(const Table&) = delete;

        void change_file(const std::string&);
        void change_file(std::string&&) noexcept;
        
        const auto& get_table() const& noexcept;
        auto&& get_table() && noexcept;
        const auto&& get_table() const&& noexcept;
        void set_table(const table_type&);
        void set_table(table_type&&) noexcept;

        void set_expected_column_count(size_type) noexcept;
        void load(bool = false);
        void dump() const;
        void append() const;
};

inline ssv::Table::Table(const std::string& filename)
    : filename{filename}
{}

inline ssv::Table::Table(std::string&& filename)
    : filename{std::move(filename)}
{}

/// @brief Change the associated file.
/// @param new_filename Path to the new file.
/// @exception @c `std::bad_alloc` may be thrown on failure to allocate
///            memory.
inline void ssv::Table::change_file(const std::string& new_filename)
{
    filename = new_filename;
}

/// @brief Change the associated file.
/// @param new_filename Path to the new file.
inline void ssv::Table::change_file(std::string &&new_filename) noexcept
{
    filename = std::move(new_filename);
}

/// @brief Get a const lvalue reference to the table.
/// @return Const lvalue reference to the table.
inline const auto& ssv::Table::get_table() const& noexcept
{
    return table;
}

/// @brief Get an rvalue reference to the table.
/// @return rvalue reference to the table.
/// @note This can be used to move the table out of the class without
///       creating a redundant copy.
/// 
/// #### Example
/// 
/// ```c++
/// ssv::Table table_from_file("./sample.ssv");
/// auto table = std::move(table_from_file).get_table(); /* Perform operations on table */
/// ```
inline auto&& ssv::Table::get_table() && noexcept
{
    return std::move(table);
}

/// @brief Get a const rvalue reference to the table.
/// @return const rvalue reference to the table.
/// @note This can be used to move the table out of the class without
///       creating a redundant copy.
/// 
/// #### Example
/// 
/// ```c++
/// ssv::Table table_from_file("./sample.ssv");
/// const auto table = std::move(table_from_file).get_table(); /* Immutable */
/// ```
inline const auto&& ssv::Table::get_table() const&& noexcept
{
    return std::move(table);
}

/// @brief Replace the existing table with a new one.
/// @exception @c `std::bad_alloc` may be thrown on failure to allocate
///            memory.
inline void ssv::Table::set_table(const table_type& new_table)
{
    table = new_table;
}

/// @brief Replace the existing table with a new one.
inline void ssv::Table::set_table(table_type&& new_table) noexcept
{
    table = std::move(new_table);
}

/// @brief Declare the number of columns in the file. This helps pre-allocate
///        the right amount of space for each row while loading the file into
///        memory, increasing both speed and memory-efficiency.
/// 
///        The column count is only a suggestion; mismatch between the set
///        count and the actual count will not cause an error.
/// 
/// @param n Number of columns expected to be in the file.
inline void ssv::Table::set_expected_column_count(const size_type n) noexcept
{
    reserve_columns = n;
}

/// @brief Load a table from the associated file into memory.
/// 
/// @param append Set to `true` to append the loaded table to the existing
///               table. `false` by default.
/// 
/// @exception @c `std::runtime_error` may be thrown if errors occur while
///            handling the file.
/// 
///            @c `std::bad_alloc` may be thrown on failure to allocate
///            memory.
inline void ssv::Table::load(const bool append)
{
    if (!append)
        table.clear();

    std::ifstream f{filename, std::ios::in};

    if (!f.is_open())
        throw std::runtime_error(std::string{"File could not be opened: "}.append(filename));

    if (!f)
        throw std::runtime_error(std::string{"Error reading file: "}.append(filename));

    std::string line;

    if (!std::getline(f, line)) {
        // By convention, we place a newline at the beginning of the file.
        // We read it in here.
        f.close();
        return;
    }

    while (std::getline(f, line)) {
        table.emplace_back();                           // Append empty row.
        
        auto& row = table.back();
        row.reserve(reserve_columns);                   // Reserve column space.

        std::stringstream linestream(line);             /** @brief Stringstream for @c `getline()`. */

        do
            row.emplace_back();
        while (std::getline(linestream, row.back(), '\\')); // Delimiter: '\\'

        row.pop_back();
    }

    f.close();
}

/// @brief Write the contents of the table to the associated file. Overwrites
///        the existing contents of the file.
/// 
/// @exception @c `std::runtime_error` may be thrown if errors occur while
///            handling the file.
inline void ssv::Table::dump() const
{
    std::ofstream f{filename, std::ios::out | std::ios::trunc};

    if (!f.is_open())
        throw std::runtime_error(std::string{"File could not be opened: "}.append(filename));

    if (!f)
        throw std::runtime_error(std::string{"Error writing to file: "}.append(filename));

    write_table_to_file(f);

    f.close();
}

/// @brief Append the table to the associated file. The existing file contents
///        will not be modified.
/// 
/// @exception @c `std::runtime_error` may be thrown if errors occur while
///            handling the file.
inline void ssv::Table::append() const
{
    std::ofstream f{filename, std::ios::app};

    if (!f.is_open())
        throw std::runtime_error(std::string{"File could not be opened: "}.append(filename));

    if (!f)
        throw std::runtime_error(std::string{"Error writing to file: "}.append(filename));

    bool retFlag;
    write_table_to_file(f);

    f.close();
}

/// @brief Write the table contents to a file, properly formatted. The
///        contents are written from the current seek position of the file
///        stream.
/// 
/// @param f File stream opened in write or append mode.
inline void ssv::Table::write_table_to_file(std::ofstream &f) const
{
    if (table.empty())
        return;

    f << '\n'; // Delimiter for the previous line (convention if file was empty).

    const auto last_row_it = table.cend() - 1;

    for (auto row_it = table.cbegin(); row_it != last_row_it; ++row_it)
    {
        if (row_it->empty())
            continue;

        const auto last_col_it = row_it->cend() - 1;
        for (auto col_it = row_it->cbegin(); col_it != last_col_it; ++col_it)
            f << *col_it << '\\'; // Insert column delimiter

        f << *last_col_it << '\n'; // Insert row delimiter
    }

    if (!last_row_it->empty())
    {
        const auto last_col_it = last_row_it->cend() - 1;
        for (auto col_it = last_row_it->cbegin(); col_it != last_col_it; ++col_it)
            f << *col_it << '\\'; // Insert column delimiter

        f << *last_col_it; // No delimiter (EOF)
    }
}
