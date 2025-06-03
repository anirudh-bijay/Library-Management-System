#include "users.h"

#include <cstdlib>

services::Users::row_type services::Users::get(const difference_type n) const
{
    const auto& row = table.at(n);

    auto password = row.at(3);
    for (auto it = row.cbegin() + 4; it != row.cend(); ++it)
        password.append("\\").append(*it);

    return row_type(
        std::strtoull(row.at(0).c_str(), nullptr, 10),
        row.at(1),
        UserType(std::strtoul(row.at(2).c_str(), nullptr, 10)),
        password
    );
}
