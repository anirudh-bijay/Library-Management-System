#include "accounts.h"

services::Accounts::row_type services::Accounts::get(const difference_type n) const
{
    return row_type(
        get <0>(n), get <1>(n)
    );
}