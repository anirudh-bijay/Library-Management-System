#include "books.h"

#include <cstdlib>

services::Books::row_type services::Books::get(const difference_type n) const
{
    const auto& row = table.at(n);

    return row_type(lms::Book{
        std::strtoull(row.at(0).c_str(), nullptr, 10),
        row.at(1),
        row.at(2),
        row.at(3),
        row.at(4),
        std::strtoul(row.at(5).c_str(), nullptr, 10),
        lms::Book::status_type(std::strtoul(row.at(6).c_str(), nullptr, 10))
    });
}
